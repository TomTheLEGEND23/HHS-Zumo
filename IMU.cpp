#include "IMU.h"

IMU::IMU(float x, float y):status(false), accelBiasX(0.0), accelBiasY(0.0), accelBiasZ(0.0), gyroBiasX(0.0), gyroBiasY(0.0), gyroBiasZ(0.0), magCalX(x), magCalY(y) {
}

bool IMU::init() {
  Wire.begin();
  imu.enableDefault();
  if (imu.init()) {
    status = true;
  }

  //Doe 100 tet om gemiddelde afwijking te krijgen van sensor bias
  for (int i = 0; i < 100; i++) {
    imu.read();  // Lees nieuwste data van de IMU
    accelBiasX += imu.a.x * 0.061 * 9.80665 / 1000.0;
    accelBiasY += imu.a.y * 0.061 * 9.80665 / 1000.0;
    accelBiasZ += imu.a.z * 0.061 * 9.80665 / 1000.0;

    gyroBiasX += (imu.g.x * 8.75 / 1000.0);
    gyroBiasY += (imu.g.y * 8.75 / 1000.0);
    gyroBiasZ += (imu.g.z * 8.75 / 1000.0);
  }
  // Deel door 100 voor gemiddelde
  accelBiasX /= 100.0;
  accelBiasY /= 100.0;
  accelBiasZ = accelBiasZ / 100.0 - 9.80665;

  gyroBiasX /= 100;
  gyroBiasY /= 100;
  gyroBiasZ /= 100;
}

float IMU::accelX() {
  imu.enableDefault();
  Wire.begin();
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;  //Accelerometer data x-axis

  return ax;
}

float IMU::accelY() {
  imu.enableDefault();
  Wire.begin();
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;  //Accelerometer data x-axis

  return ay;
}

float IMU::accelZ() {
  imu.enableDefault();
  Wire.begin();
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;  //Accelerometer data x-axis
  return az;
}

float IMU::gyroX() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float gx = imu.g.x * 8.75 / 1000.0 - gyroBiasX;

  return gx;
}

float IMU::gyroY() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float gy = imu.g.y * 8.75 / 1000.0 - gyroBiasY;

  return gy;
}

float IMU::gyroZ() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float gz = imu.g.z * 8.75 / 1000.0 - gyroBiasZ;

  return gz;
}

float IMU::compassHeading() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;  //Accelerometer data x-axis
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;  //Accelerometer data y-axis
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;  //Accelerometer data z-axis

  // Magnetometer data (in Gauss)
  float mx = imu.m.x - magCalX;  // Magnetometer X-axis with calibration
  float my = imu.m.y - magCalY;  // Magnetometer Y-axis with calibration

  // Normaliseer accelerometer data
  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ax_n = ax / accNorm;
  float ay_n = ay / accNorm;
  float az_n = az / accNorm;

  // Bereken pitch and roll from accelerometer data
  float pitch = asin(-ax_n);       // Forward/Backward tilt
  float roll = atan2(ay_n, az_n);  // Sideways tilt

  // Tilt compensatie for magnetometer data (gebruikt alleen x en y waardes)
  float mx_comp = mx * cos(pitch);                               // gebruik alleen mx for tilt compensatie
  float my_comp = mx * sin(roll) * sin(pitch) + my * cos(roll);  // gebruik alleen my for tilt compensatie

  // Bereken de richting (heading) in graden ten opzichte van magnetisch noorden
  float heading = atan2(-my_comp, mx_comp) * (180.0 / PI);
  if (heading < 0) heading += 360.0;

  return heading;
}

float IMU::pitch() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;  //Accelerometer data x-axis
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;  //Accelerometer data y-axis
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;  //Accelerometer data z-axis

  // Normaliseer accelerometer data
  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ax_n = ax / accNorm;

  // Bereken pitch and roll from accelerometer data
  float pitch = asin(-ax_n);  // Forward/Backward tilt
  return pitch;
}

float IMU::roll() {
  imu.read();  // Lees nieuwste data van de IMU

  // Converteer analoge waardes volgens datseheet LSM303D naar m/s2
  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;  //Accelerometer data x-axis
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;  //Accelerometer data y-axis
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;  //Accelerometer data z-axis

  // Normaliseer accelerometer data
  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ay_n = ay / accNorm;
  float az_n = az / accNorm;

  // Bereken pitch and roll from accelerometer data
  float roll = atan2(ay_n, az_n);  // Sideways tilt
  return roll;
}

bool IMU::fetchStatus() {
  return status;
}