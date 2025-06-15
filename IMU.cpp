#include "IMU.h"

// Constructor for the IMU class.
// Initializes the IMU with given magnetometer calibration values.
IMU::IMU()
  : status(false), accelBiasX(0.0), accelBiasY(0.0), accelBiasZ(0.0), gyroBiasX(0.0), gyroBiasY(0.0), gyroBiasZ(0.0) {
}

// Initializes the IMU sensor and calculates accelerometer and gyroscope bias.
bool IMU::init() {
  Wire.begin();
  imu.enableDefault();
  if (imu.init()) {
    status = true;
  }

  for (int i = 0; i < 100; i++) {
    imu.read();
    accelBiasX += imu.a.x * 0.061 * 9.80665 / 1000.0;
    accelBiasY += imu.a.y * 0.061 * 9.80665 / 1000.0;
    accelBiasZ += imu.a.z * 0.061 * 9.80665 / 1000.0;

    gyroBiasX += imu.g.x * 8.75 / 1000.0;
    gyroBiasY += imu.g.y * 8.75 / 1000.0;
    gyroBiasZ += imu.g.z * 8.75 / 1000.0;
  }

  accelBiasX /= 100.0;
  accelBiasY /= 100.0;
  accelBiasZ = accelBiasZ / 100.0 - 9.80665;

  gyroBiasX /= 100;
  gyroBiasY /= 100;
  gyroBiasZ /= 100;

  return status;
}

// Returns corrected acceleration on the X-axis.
float IMU::accelX() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
}

// Returns corrected acceleration on the Y-axis.
float IMU::accelY() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
}

// Returns corrected acceleration on the Z-axis.
float IMU::accelZ() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;
}

// Returns corrected gyroscope value on the X-axis.
float IMU::gyroX() {
  imu.read();
  return imu.g.x * 8.75 / 1000.0 - gyroBiasX;
}

// Returns corrected gyroscope value on the Y-axis.
float IMU::gyroY() {
  imu.read();
  return imu.g.y * 8.75 / 1000.0 - gyroBiasY;
}

// Returns corrected gyroscope value on the Z-axis.
float IMU::gyroZ() {
  imu.read();
  return imu.g.z * 8.75 / 1000.0 - gyroBiasZ;
}

float IMU::pitch() {
  imu.read();

  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;

  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ax_n = ax / accNorm;

  return asin(-ax_n);
}

// Calculates roll angle based on accelerometer data.
float IMU::roll() {
  imu.read();

  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;

  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ay_n = ay / accNorm;
  float az_n = az / accNorm;

  return atan2(ay_n, az_n);
}

// Returns the initialization status of the IMU.
bool IMU::fetchStatus() {
  return status;
}
