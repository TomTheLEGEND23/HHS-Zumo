/**
 * @file IMU.h
 * @brief Definitie van de IMU-klasse voor het uitlezen van sensorwaarden van de Zumo32U4 IMU.
 */

#include <Zumo32U4.h>
#include <Wire.h>

/**
 * @class IMU
 * @brief Klasse voor het werken met de Zumo32U4 IMU-sensor.
 * 
 * Deze klasse bevat functies om versnelling, gyroscoopdata, kompasrichting en oriëntatie
 * (pitch en roll) te verkrijgen, met correctie voor bias en kalibratie.
 */
class IMU {
public:
  /**
   * @brief Constructor voor de IMU-klasse.
   * @param x Kalibratie-offset voor magnetometer op de X-as.
   * @param y Kalibratie-offset voor magnetometer op de Y-as.
   */
  IMU(float x, float y);

  /**
   * @brief Initialiseert de IMU en bepaalt bias-waarden.
   * @return true als initialisatie is gelukt, anders false.
   */
  bool init();

  /**
   * @brief Haalt gecorrigeerde versnelling op de X-as op.
   * @return Versnelling in m/s².
   */
  float accelX();

  /**
   * @brief Haalt gecorrigeerde versnelling op de Y-as op.
   * @return Versnelling in m/s².
   */
  float accelY();

  /**
   * @brief Haalt gecorrigeerde versnelling op de Z-as op.
   * @return Versnelling in m/s².
   */
  float accelZ();

  /**
   * @brief Haalt gecorrigeerde gyroscoopwaarde op de X-as op.
   * @return Hoeksnelheid in graden per seconde.
   */
  float gyroX();

  /**
   * @brief Haalt gecorrigeerde gyroscoopwaarde op de Y-as op.
   * @return Hoeksnelheid in graden per seconde.
   */
  float gyroY();

  /**
   * @brief Haalt gecorrigeerde gyroscoopwaarde op de Z-as op.
   * @return Hoeksnelheid in graden per seconde.
   */
  float gyroZ();

  /**
   * @brief Berekent de richting t.o.v. het magnetisch noorden.
   * @return Richting (heading) in graden.
   */
  float compassHeading();

  /**
   * @brief Haalt de status van de sensorinitialisatie op.
   * @return true als succesvol geïnitialiseerd, anders false.
   */
  bool fetchStatus();

  /**
   * @brief Berekent de pitchhoek (voor-/achterwaartse kanteling).
   * @return Pitch in radialen.
   */
  float pitch();

  /**
   * @brief Berekent de rollhoek (zijwaartse kanteling).
   * @return Roll in radialen.
   */
  float roll();

private:
  Zumo32U4IMU imu; ///< IMU-object van de Zumo32U4 bibliotheek
  bool status;     ///< Status van de initialisatie

  // Versnellingsbiasen (in m/s²)
  float accelBiasX;
  float accelBiasY;
  float accelBiasZ;

  // Gyroscoopbiasen (in °/s)
  float gyroBiasX;
  float gyroBiasY;
  float gyroBiasZ;

  // Magnetometerkalibratie
  float magCalX;
  float magCalY;
};
