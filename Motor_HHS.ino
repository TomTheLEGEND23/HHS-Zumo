#include <Wire.h>
#include <Zumo32U4.h>
#include "Motor_HHS.h"

/**
 * @brief Motorobject voor de linker motor.
 */
Motor_HHS motorLinks("links", 10, 16);

/**
 * @brief Motorobject voor de rechter motor.
 */
Motor_HHS motorRechts("rechts", 9, 15);

/**
 * @brief Arduino setup-functie. Initialiseert de motoren en demonstreert beweging.
 */
void setup() {
  motorLinks.init(); ///< Initialiseer de linker motor.
  motorRechts.init(); ///< Initialiseer de rechter motor.
  motorLinks.setSnelheid(0); ///< Stel de snelheid van de linker motor in op 0.
  motorRechts.setSnelheid(0); ///< Stel de snelheid van de rechter motor in op 0.

  // Voorbeeld om de robot een stukje te laten rijden
  delay(2000); ///< Wacht 20 seconden.
  motorLinks.setSnelheid(-225); ///< Stel de snelheid van de linker motor in op -225.
  motorRechts.setSnelheid(-225); ///< Stel de snelheid van de rechter motor in op -225.
  delay(2000); ///< Wacht 20 seconden.
  motorLinks.setSnelheid(0); ///< Stop de linker motor.
  motorRechts.setSnelheid(0); ///< Stop de rechter motor.
}

/**
 * @brief Arduino loop-functie. Momenteel leeg.
 */
void loop() {

}