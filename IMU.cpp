#include "IMU.h"

/**
 * @brief Constructor voor de IMU klasse.
 * 
 * Initialiseert de IMU met gegeven magnetometer calibratie waarden.
 * 
 * @param x Calibratie waarde voor de magnetometer X-as.
 * @param y Calibratie waarde voor de magnetometer Y-as.
 */
IMU::IMU(float x, float y)
  : status(false),
    accelBiasX(0.0), accelBiasY(0.0), accelBiasZ(0.0),
    gyroBiasX(0.0), gyroBiasY(0.0), gyroBiasZ(0.0),
    magCalX(x), magCalY(y) {
}

/**
 * @brief Initialiseert de IMU sensor.
 * 
 * Voert de initialisatie van de IMU uit en berekent de bias van de accelerometer
 * en gyroscoop door 100 metingen uit te voeren.
 * 
 * @return true als de initialisatie succesvol was, anders false.
 */
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

/**
 * @brief Geeft de gecorrigeerde versnelling op de X-as.
 * 
 * Berekent de versnelling op de X-as na correctie voor bias.
 * 
 * @return De versnelling in m/s² op de X-as.
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
 * Berekent de versnelling op de Y-as na correctie voor bias.
 * 
 * @return De versnelling in m/s² op de Y-as.
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
 * Berekent de versnelling op de Z-as na correctie voor bias.
 * 
 * @return De versnelling in m/s² op de Z-as.
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
 * Berekent de hoeksnelheid op de X-as na correctie voor bias.
 * 
 * @return De hoeksnelheid in graden per seconde op de X-as.
 */
float IMU::gyroX() {
  imu.read();
  return imu.g.x * 8.75 / 1000.0 - gyroBiasX;
}

/**
 * @brief Geeft de gecorrigeerde gyroscoopwaarde op de Y-as.
 * 
 * Berekent de hoeksnelheid op de Y-as na correctie voor bias.
 * 
 * @return De hoeksnelheid in graden per seconde op de Y-as.
 */
float IMU::gyroY() {
  imu.read();
  return imu.g.y * 8.75 / 1000.0 - gyroBiasY;
}

/**
 * @brief Geeft de gecorrigeerde gyroscoopwaarde op de Z-as.
 * 
 * Berekent de hoeksnelheid op de Z-as na correctie voor bias.
 * 
 * @return De hoeksnelheid in graden per seconde op de Z-as.
 */
float IMU::gyroZ() {
  imu.read();
  return imu.g.z * 8.75 / 1000.0 - gyroBiasZ;
}

/**
 * @brief Berekent de kompasrichting met tiltcompensatie.
 * 
 * Berekent de richting ten opzichte van het magnetisch noorden door gebruik
 * te maken van accelerometer- en magnetometerdata.
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
 * Berekent de pitchhoek (hoek van de accelerometer ten opzichte van de verticale)
 * uit de accelerometerdata.
 * 
 * @return Pitchhoek in radialen.
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
 * Berekent de rollhoek (hoek van de accelerometer ten opzichte van de horizontale)
 * uit de accelerometerdata.
 * 
 * @return Rollhoek in radialen.
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
 * Geeft terug of de IMU correct geïnitialiseerd is.
 * 
 * @return true als succesvol geïnitialiseerd, anders false.
 */
bool IMU::fetchStatus() {
  return status;
}
