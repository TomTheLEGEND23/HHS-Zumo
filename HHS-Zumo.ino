#include "IMU.h"

void setup() {
  IMU imu(-2583.0, -2939.0);
  Serial.println(imu.compassHeading());
}

void loop() {
  // put your main code here, to run repeatedly:

}
