#include <ContinuousServo.h>
#include <ros.h>
#include <geometry_msgs/Quaternion.h>
#include <math.h>

#define XGAIN 40 //tilt
#define ZGAIN 40


ros::NodeHandle nh;
ContinuousServo pan(3);//pan
ContinuousServo tilt(6);//tilt

bool first = true;
double origX,origZ;
double curX,curZ;
double nextX,nextZ;
void adjustAngleCb(const geometry_msgs::Quaternion& q)
{
  //x is tilt
  //z is  pan
  if (first){
    origX = q.x * XGAIN;
    origZ = q.z * ZGAIN;
    first = false;
  }
  else{
  
    double nextX = q.x * XGAIN - origX;
    double nextZ = q.z * ZGAIN - origZ;
    
    if( fabs(curX - nextX) < 0.8*XGAIN && fabs(curZ - nextZ) < 0.8*ZGAIN){ 
      pan.step(nextZ - curZ);
      tilt.step(nextX - curX);
    }
    
    curX = nextX;
    curZ = nextZ;    
  }
}
ros::Subscriber<geometry_msgs::Quaternion> sub("/get_quat", &adjustAngleCb);

  int curPos1 =0;
  int curPos2 =0;
  int nextPos1;
  int nextPos2;
void setup() {
  // put your setup code here, to run once:
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  
  nh.subscribe(sub);
}

void loop() {
  // put your main code here, to run repeatedly:
 /* if(Serial.available() > 0)
  {
    nextPos1 = Serial.parseInt();
    nextPos2 = Serial.parseInt();
    s.step(nextPos1 - curPos1);
    s2.step(nextPos2-curPos2);
    curPos1 = nextPos1;
    curPos2 = nextPos2;
  }*/
  nh.spinOnce();
  delay(10);
}
