#include <ContinuousServo.h>
#include <ros.h>
#include <geometry_msgs/Quaternion.h>

ros::NodeHandle nh;
ContinuousServo s1(9);
ContinuousServo s2(10);

bool first = true;
double origX,origZ;
double curX,curZ;
double nextX,nextZ;
void adjustAngleCb(const geometry_msgs::Quaternion& q)
{
  //x is tilt
  //z is pan
  if (first){
    origX = q.x * 20;
    origZ = q.z * 20 + 20;
    first = false;
  }
  else{
  
    double nextX = q.x * 20 - origX;
    double nextZ = q.z * 20 + 20- origZ;
    
    s1.step(nextZ - curZ);
    s2.step(nextX - curX);
    
    curX = nextX;
    curZ = nextZ;    
  }
}
ros::Subscriber<geometry_msgs::Quaternion> sub("get_quat", &adjustAngleCb);

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
  delay(1);
}
