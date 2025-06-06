#include "Arduino.h"
#include <Zumo32U4.h>
#include <Wire.h>

class IMU {
public:
  IMU();
  bool init();
  float accelX();
  float accelY();
  float accelZ();
  float gyroX();
  float gyroY();
  float gyroZ();
  bool fetchStatus();
  float pitch();
  float roll();

private:
  Zumo32U4IMU imu;
  bool status;

  float accelBiasX;
  float accelBiasY;
  float accelBiasZ;

  float gyroBiasX;
  float gyroBiasY;
  float gyroBiasZ;
};
