#include "IMU.h"
IMU imu(-2538, -2939.0);

void setup() {
  Serial.begin(9600);
  imu.init();
}

void loop() {
  //Serial.println(imu.accelZ());
  Serial.println(imu.compassHeading());
  delay(500);

}
