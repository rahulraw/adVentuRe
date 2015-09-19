/**********************************************************/
//               Robot Racer Controller                   //
//               Written By: Sirui Song                   //
//               Last Updated: Jun 30, 2014               //
/**********************************************************/
//#define TEST_OUTPUT

//Import Libraries
#include <Servo.h>       
#include <ros.h>

//servo
Servo throttleServo, steerServo,   brakeServo;
#define SERVO_THROTTLE_PIN    10
#define SERVO_STEER_PIN       11
#define SERVO_BRAKE_PIN       12
#define MANUAL_MAX            1575
#define MANUAL_REV_MAX        1375
#define THROTTLE_MAX          1675  // 1675 is 10 m/s, max. is 2000
#define THROTTLE_REV_MAX      1300  // Under 1300 is too fast, min. is 1000
#define MAX_RC_VAL            1738
#define MIN_RC_VAL            304
#define REST_RC_VAL           874
#define MAX_STEER_VAL         1732
#define MIN_STEER_VAL         306
#define REST_STEER_VAL        990
#define MAX_STEERING          2000
#define MIN_STEERING          1000
#define NEUTRAL               1500
#define STEER_NEUTRAL         1460
#define EC_BAUD_RATE          115200

unsigned int throttle_auto  = 1500;
unsigned int steering_auto  = 1500;
unsigned int throttle_rc    = 1500;
unsigned int steering_rc    = 1500;
unsigned int brake_rc       = 1500;
unsigned int throttle       = 1500;
unsigned int steering       = 1500;
unsigned int brake          = 1500;

unsigned int prev_steering = 1500;  

// Constants for RC mode
float x_reverse;
float x_forward;
float x_left;
float x_right;

bool enable = false;

//Define Analog Input Pin
#define BATT_VIN_1_PIN        A0
#define BATT_VIN_2_PIN        A1

/**********************************************************/
//                     RC Read In                         //
/**********************************************************/
int ppm[8];
void RC_read ()
{
  if (Serial.available() > 0){
    int count00       = 0;
    int buffer_size   = 0;
    int incomingByte  = 0;
    while (count00 < 50){
      // throw away extra bytes
      if (Serial.available() > 16) {
        incomingByte = Serial.read();
        count00 = 0;
      } 
      else {
        count00++;
      }

      // wait for a full transmition
      if (Serial.available() != buffer_size)
        count00 = 0;

      // make sure we are at the end of a transmition
      buffer_size = Serial.available();
    }

    if (Serial.available() == 16) {
      int ppm_data  = 0;
      int ppm_ch    = 0;
      for (unsigned int i=0; i<16; i++){
        incomingByte = Serial.read();
        if (i >= 2) {
          if (i%2 == 0) {
            ppm_ch        = (incomingByte >> 3);
            incomingByte -= (ppm_ch << 3);
            ppm_data      = (incomingByte << 8);

            if (ppm_ch > 7) break;
          } 
          else {
            ppm_data     += incomingByte;
            ppm[ppm_ch]   = ppm_data;
          }
        }
      }
    }
  }
}

//ESC Setup
void setup_ESC()
{
  long tmp_time = millis();

  while ( (millis() - tmp_time) <5000)
  {
    throttleServo.writeMicroseconds(1500);
  }
}

void setup()
{  
  Serial.begin(EC_BAUD_RATE);
  
  throttleServo.attach(SERVO_THROTTLE_PIN);
  steerServo.attach(SERVO_STEER_PIN);
  brakeServo.attach(SERVO_BRAKE_PIN);

  setup_ESC();

  // Constants for RC mode control
  x_reverse = (MIN_RC_VAL - REST_RC_VAL)/(MANUAL_REV_MAX - NEUTRAL);
  x_forward = (MAX_RC_VAL - REST_RC_VAL)/(MANUAL_MAX - NEUTRAL);
  x_left = (MAX_STEER_VAL - REST_STEER_VAL)/(MAX_STEERING - STEER_NEUTRAL);
  x_right = (MIN_STEER_VAL - REST_STEER_VAL)/(MIN_STEERING - STEER_NEUTRAL);
}

long CurrentTime = 0;
long PrevTime = micros();
long timeDiff = CurrentTime - PrevTime;
float filterDegree = 0.70; // Amount of filtering, on a scale of 0 to 1 where 0 is no filtering and 1 is max filtering

void loop()
{
  RC_read ();

  boolean Estop_on = true;

  if (ppm[4] < 700) 
  {
    Estop_on = true;
  }
  else
  {
    Estop_on = false;
  }

  if (Estop_on) {
    //Estop state:  
    while(throttle >= 1500)
    {
      throttle -= 25;
      throttleServo.writeMicroseconds(throttle);
      delay(100);

    }//end while 
    steering = prev_steering;
    brake = 2000;

    //Reset Integrator 
  }
  else {
    //RC state
    //Transfer PWM, with a max output limited

    // Equation was formed using previously defined formulas
    if(ppm[2] >= REST_RC_VAL)
    {
      throttle_rc = round((ppm[2] - REST_RC_VAL) / x_forward) + NEUTRAL;
    }
    else
    {
      throttle_rc = round((ppm[2] - REST_RC_VAL) / x_reverse) + NEUTRAL;
    }

    // Many of the numerical values were previously defined values
    int steering_read = ppm[3];

    // Give range for steering so that it stays neutral through fluctuations in RC values
    if(steering_read >= 960 && steering_read <= 1020)
    {  
      steering_rc = STEER_NEUTRAL;
      //steering_rc = round(-1*(steering_read-960)/1.308)+NEUTRAL;
    }
    else if(steering_read > 1020)
    {
      steering_rc = round(-1 * (steering_read - REST_STEER_VAL) / x_left) + STEER_NEUTRAL;
    }
    else
      steering_rc = round(-1 * (steering_read - REST_STEER_VAL) / x_right) + STEER_NEUTRAL;
    
    brake_rc = round((ppm[0] - 1022) / 1.432) + NEUTRAL;

    throttle = throttle_rc;
    steering = steering_rc;
    brake = brake_rc;
  }

  if (throttle > THROTTLE_MAX)
    throttle = THROTTLE_MAX;
  else if (throttle < THROTTLE_REV_MAX)
    throttle = THROTTLE_REV_MAX;
    
  if (steering > MAX_STEERING)
    steering = MAX_STEERING;
  else if (steering < MIN_STEERING)
    steering = MIN_STEERING; 

  steerServo.writeMicroseconds(steering);
  throttleServo.writeMicroseconds(throttle);
  brakeServo.writeMicroseconds(brake);
  delay(10);
  
  prev_steering = steering;
}
