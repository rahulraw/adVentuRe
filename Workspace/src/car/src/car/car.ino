#include <ros.h>
#include <Servo.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Int32.h>

#define SERVO_THROTTLE_PIN  10
#define SERVO_STEER_PIN     11
#define SERVO_BRAKE_PIN     12
#define X_AXIS_INDEX        0
#define Y_AXIS_INDEX        1
#define ESTOP               0
#define NEUTRAL             1500
#define MIN_STEER           1000
#define MAX_STEER           2000
#define MIN_SPEED           1400
#define MAX_SPEED           1540

Servo throttleServo, steerServo, brakeServo;
int x_input = 1500, y_input = 1500;
boolean emergency, old = false;

void joyCb(const sensor_msgs::Joy& joy)
{
  x_input = int(joy.axes[X_AXIS_INDEX] * -500) + NEUTRAL;
  y_input = int(joy.axes[Y_AXIS_INDEX] * 500) + NEUTRAL;
  
  if (joy.buttons[ESTOP] && !old)
  {
    emergency = true;
    old = true;
  }
  else if (joy.buttons[ESTOP] && old)
  {
    emergency = false;
    old = false;
  }
  
  if (x_input > MAX_STEER)
  {
    x_input = MAX_STEER;
  }
  else if (x_input < MIN_STEER)
  {
    x_input = MIN_STEER; 
  }
}

// ROS setup
ros::NodeHandle nh;

std_msgs::Int32 steer_msg;
std_msgs::Int32 speed_msg;

ros::Subscriber<sensor_msgs::Joy> joySub("joy", &joyCb);

ros::Publisher steer_cmd("/arduino/steer_cmd", &steer_msg);
ros::Publisher speed_cmd("/arduino/speed_cmd", &speed_msg);


void setup_ESC()
{
  long temp_time = millis();
  
  while ((millis() - temp_time) < 5000)
  {
    throttleServo.writeMicroseconds(NEUTRAL);
    steerServo.writeMicroseconds(NEUTRAL);
  }
}

void setup()
{
  nh.getHardware()->setBaud(115200);
  nh.initNode();

  nh.advertise(steer_cmd);
  nh.advertise(speed_cmd);
  
  nh.subscribe(joySub);
  
  throttleServo.attach(SERVO_THROTTLE_PIN);
  steerServo.attach(SERVO_STEER_PIN);
  brakeServo.attach(SERVO_BRAKE_PIN);
  
  delay(1000);
  
  setup_ESC();
}

void loop()
{
  if (emergency)
  {
    x_input = NEUTRAL;
    y_input = NEUTRAL;
  }
  
  int speed_signal;
  
  if (y_input >= 1500)
  {
    speed_signal = int(((MAX_SPEED - NEUTRAL)/(2000.0 - NEUTRAL)) * (y_input - NEUTRAL)) + NEUTRAL;
  }
  else if (y_input < 1500)
  {
    speed_signal = int((-(NEUTRAL - MIN_SPEED)/(NEUTRAL - 1000.0)) * (NEUTRAL - y_input)) + NEUTRAL;
  }
    
  if (speed_signal > MAX_SPEED)
  {
    speed_signal = MAX_SPEED;
  }
  else if (speed_signal < MIN_SPEED)
  {
    speed_signal = MIN_SPEED;
  }
  
  steer_msg.data = x_input;
  speed_msg.data = speed_signal;
  
  steerServo.writeMicroseconds(x_input);
  throttleServo.writeMicroseconds(speed_signal);
  
  nh.spinOnce();
  steer_cmd.publish(&steer_msg);
  speed_cmd.publish(&speed_msg);
  delay(5);
}
