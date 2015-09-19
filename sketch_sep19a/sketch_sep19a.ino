#include <ContinuousServo.h>
ContinuousServo s(9);
ContinuousServo s2(10);
  int curPos1 =0;
  int curPos2 =0;
  int nextPos1;
  int nextPos2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    nextPos1 = Serial.parseInt();
    nextPos2 = Serial.parseInt();
    s.step(nextPos1 - curPos1);
    s2.step(nextPos2-curPos2);
    curPos1 = nextPos1;
    curPos2 = nextPos2;
  }
}
