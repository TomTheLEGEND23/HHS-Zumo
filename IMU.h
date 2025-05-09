#include <Zumo32U4.h>
#include <Wire.h>

/**
 * @class IMU
 * @brief Klasse voor het werken met de Zumo32U4 IMU-sensor.
 * 
 * Deze klasse biedt methoden om de versnelling, gyroscoopdata, kompasrichting en oriëntatie
 * (pitch en roll) te verkrijgen, met correctie voor bias en kalibratie.
 */
class IMU {
public:
  /**
   * @brief Constructor voor de IMU-klasse.
   * @param x Kalibratie-offset voor de magnetometer op de X-as.
   * @param y Kalibratie-offset voor de magnetometer op de Y-as.
   */
  IMU(float x, float y);

  /**
   * @brief Initialiseert de IMU-sensor en bepaalt de benodigde bias-waarden.
   * 
   * Deze functie voert een initiële meting uit om de bias-waarden voor versnelling en gyroscoop
   * te berekenen en op te slaan. Het retourneert 'true' als de initialisatie is geslaagd, anders 'false'.
   * @return true als de initialisatie succesvol is, anders false.
   */
  bool init();

  /**
   * @brief Haalt de gecorrigeerde versnelling op de X-as op.
   * 
   * Deze functie leest de versnelling van de X-as uit en corrigeert voor de vooraf berekende versnellingbias.
   * @return Versnelling op de X-as in meter per seconde kwadraat (m/s²).
   */
  float accelX();

  /**
   * @brief Haalt de gecorrigeerde versnelling op de Y-as op.
   * 
   * Deze functie leest de versnelling van de Y-as uit en corrigeert voor de vooraf berekende versnellingbias.
   * @return Versnelling op de Y-as in meter per seconde kwadraat (m/s²).
   */
  float accelY();

  /**
   * @brief Haalt de gecorrigeerde versnelling op de Z-as op.
   * 
   * Deze functie leest de versnelling van de Z-as uit en corrigeert voor de vooraf berekende versnellingbias.
   * @return Versnelling op de Z-as in meter per seconde kwadraat (m/s²).
   */
  float accelZ();

  /**
   * @brief Haalt de gecorrigeerde gyroscoopwaarde op de X-as op.
   * 
   * Deze functie leest de hoeksnelheid van de X-as uit en corrigeert voor de vooraf berekende gyroscoopbias.
   * @return Hoeksnelheid op de X-as in graden per seconde (°/s).
   */
  float gyroX();

  /**
   * @brief Haalt de gecorrigeerde gyroscoopwaarde op de Y-as op.
   * 
   * Deze functie leest de hoeksnelheid van de Y-as uit en corrigeert voor de vooraf berekende gyroscoopbias.
   * @return Hoeksnelheid op de Y-as in graden per seconde (°/s).
   */
  float gyroY();

  /**
   * @brief Haalt de gecorrigeerde gyroscoopwaarde op de Z-as op.
   * 
   * Deze functie leest de hoeksnelheid van de Z-as uit en corrigeert voor de vooraf berekende gyroscoopbias.
   * @return Hoeksnelheid op de Z-as in graden per seconde (°/s).
   */
  float gyroZ();

  /**
   * @brief Berekent de richting ten opzichte van het magnetisch noorden.
   * 
   * Deze functie gebruikt de versnellings- en magnetometerdata om de richting (heading) ten opzichte
   * van het magnetisch noorden te berekenen met behulp van tiltcompensatie.
   * @return Richting (heading) in graden (0° tot 360°).
   */
  float compassHeading();

  /**
   * @brief Haalt de status van de IMU-sensorinitialisatie op.
   * 
   * Deze functie retourneert de status van de sensorinitialisatie (of de sensor succesvol is geïnitialiseerd).
   * @return true als succesvol geïnitialiseerd, anders false.
   */
  bool fetchStatus();

  /**
   * @brief Berekent de pitchhoek (voor-/achterwaartse kanteling).
   * 
   * Deze functie berekent de pitchhoek van de IMU-sensor op basis van de versnellingsdata.
   * @return Pitchhoek in radialen.
   */
  float pitch();

  /**
   * @brief Berekent de rollhoek (zijwaartse kanteling).
   * 
   * Deze functie berekent de rollhoek van de IMU-sensor op basis van de versnellingsdata.
   * @return Rollhoek in radialen.
   */
  float roll();

private:
  Zumo32U4IMU imu; ///< Het IMU-object van de Zumo32U4-bibliotheek.
  bool status;     ///< De status van de sensorinitialisatie.

  float accelBiasX; ///< Versnellingsbias op de X-as in m/s².
  float accelBiasY; ///< Versnellingsbias op de Y-as in m/s².
  float accelBiasZ; ///< Versnellingsbias op de Z-as in m/s².

  float gyroBiasX;  ///< Gyroscoopbias op de X-as in °/s.
  float gyroBiasY;  ///< Gyroscoopbias op de Y-as in °/s.
  float gyroBiasZ;  ///< Gyroscoopbias op de Z-as in °/s.

  float magCalX;    ///< Kalibratie-offset voor de magnetometer op de X-as.
  float magCalY;    ///< Kalibratie-offset voor de magnetometer op de Y-as.
};
