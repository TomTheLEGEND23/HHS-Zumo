/**
 * @file main.ino
 * @brief Voorbeeld van het initialiseren van de IMU-klasse en het gebruik ervan in een Arduino-programma.
 */

#include "IMU.h"

// Maak een IMU-object aan met magnetometer kalibratie-offsets (X, Y)
IMU imu(-1945, -1408.0);

/**
 * @brief Setup-functie die één keer wordt uitgevoerd bij het opstarten.
 * 
 * Initialiseert de IMU-sensor en voert bias-kalibratie uit.
 */
void setup() {
  imu.init();
}

/**
 * @brief Loop-functie die herhaaldelijk wordt uitgevoerd na setup().
 * 
 * Momenteel leeg, maar hier kunnen IMU-metingen zoals versnelling, hoeksnelheid of richting worden verwerkt.
 */
void loop() {
  // Hier kun je functies zoals imu.accelX(), imu.pitch(), imu.compassHeading(), enz. aanroepen.
}
