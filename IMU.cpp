#include "IMU.h"

/**
 * @brief Constructor voor de IMU-klasse.
 * 
 * @param x Kalibratie-offset voor de magnetometer op de X-as.
 * @param y Kalibratie-offset voor de magnetometer op de Y-as.
 */
IMU::IMU(float x, float y)
  : status(false),
    accelBiasX(0.0), accelBiasY(0.0), accelBiasZ(0.0),
    gyroBiasX(0.0), gyroBiasY(0.0), gyroBiasZ(0.0),
    magCalX(x), magCalY(y) {
}

/**
 * @brief Initialiseert de IMU-sensor en berekent bias-waarden.
 * 
 * @return true als initialisatie succesvol was, anders false.
 */
bool IMU::init() {
  Wire.begin();
  imu.enableDefault();
  if (imu.init()) {
    status = true;
  }

  // Voer 100 metingen uit om gemiddelde bias te bepalen
  for (int i = 0; i < 100; i++) {
    imu.read();
    accelBiasX += imu.a.x * 0.061 * 9.80665 / 1000.0;
    accelBiasY += imu.a.y * 0.061 * 9.80665 / 1000.0;
    accelBiasZ += imu.a.z * 0.061 * 9.80665 / 1000.0;

    gyroBiasX += imu.g.x * 8.75 / 1000.0;
    gyroBiasY += imu.g.y * 8.75 / 1000.0;
    gyroBiasZ += imu.g.z * 8.75 / 1000.0;
  }

  // Gemiddelde berekenen
  accelBiasX /= 100.0;
  accelBiasY /= 100.0;
  accelBiasZ = accelBiasZ / 100.0 - 9.80665;

  gyroBiasX /= 100;
  gyroBiasY /= 100;
  gyroBiasZ /= 100;

  return status;
}

/**
 * @brief Geeft de gecorrigeerde versnelling op de X-as.
 * 
 * @return Versnelling in m/s².
 */
float IMU::accelX() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
}

/**
 * @brief Geeft de gecorrigeerde versnelling op de Y-as.
 * 
 * @return Versnelling in m/s².
 */
float IMU::accelY() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
}

/**
 * @brief Geeft de gecorrigeerde versnelling op de Z-as.
 * 
 * @return Versnelling in m/s².
 */
float IMU::accelZ() {
  imu.enableDefault();
  Wire.begin();
  imu.read();
  return imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;
}

/**
 * @brief Geeft de gecorrigeerde gyroscoopwaarde op de X-as.
 * 
 * @return Hoeksnelheid in graden per seconde.
 */
float IMU::gyroX() {
  imu.read();
  return imu.g.x * 8.75 / 1000.0 - gyroBiasX;
}

/**
 * @brief Geeft de gecorrigeerde gyroscoopwaarde op de Y-as.
 * 
 * @return Hoeksnelheid in graden per seconde.
 */
float IMU::gyroY() {
  imu.read();
  return imu.g.y * 8.75 / 1000.0 - gyroBiasY;
}

/**
 * @brief Geeft de gecorrigeerde gyroscoopwaarde op de Z-as.
 * 
 * @return Hoeksnelheid in graden per seconde.
 */
float IMU::gyroZ() {
  imu.read();
  return imu.g.z * 8.75 / 1000.0 - gyroBiasZ;
}

/**
 * @brief Berekent de kompasrichting met tiltcompensatie.
 * 
 * @return Richting in graden t.o.v. magnetisch noorden.
 */
float IMU::compassHeading() {
  imu.read();

  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;

  float mx = imu.m.x - magCalX;
  float my = imu.m.y - magCalY;

  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ax_n = ax / accNorm;
  float ay_n = ay / accNorm;
  float az_n = az / accNorm;

  float pitch = asin(-ax_n);
  float roll = atan2(ay_n, az_n);

  float mx_comp = mx * cos(pitch);
  float my_comp = mx * sin(roll) * sin(pitch) + my * cos(roll);

  float heading = atan2(-my_comp, mx_comp) * (180.0 / PI);
  if (heading < 0) heading += 360.0;

  return heading;
}

/**
 * @brief Berekent de pitchhoek op basis van versnellingsdata.
 * 
 * @return Pitch in radialen.
 */
float IMU::pitch() {
  imu.read();

  float ax = imu.a.x * 0.061 * 9.80665 / 1000.0 - accelBiasX;
  float ay = imu.a.y * 0.061 * 9.80665 / 1000.0 - accelBiasY;
  float az = imu.a.z * 0.061 * 9.80665 / 1000.0 - accelBiasZ;

  float accNorm = sqrt(ax * ax + ay * ay + az * az);
  float ax_n = ax / accNorm;

  return asin(-ax_n);
}

/**
 * @brief Berekent de rollhoek op basis van versnellingsdata.
 * 
 * @return Roll in radialen.
 */
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

/**
 * @brief Geeft de status van de initialisatie van de IMU terug.
 * 
 * @return true als succesvol geïnitialiseerd, anders false.
 */
bool IMU::fetchStatus() {
  return status;
}
