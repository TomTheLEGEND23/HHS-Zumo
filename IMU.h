#include <Zumo32U4.h>
#include <Wire.h>

class IMU {
public:
  IMU(float, float);
  bool init();
  float accelX();
  float accelY();
  float accelZ();
  float gyroX();
  float gyroY();
  float gyroZ();
  float compassHeading();
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
  float magCalX;
  float magCalY;
};