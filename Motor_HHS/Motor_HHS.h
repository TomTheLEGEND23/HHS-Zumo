#ifndef Motor_HHS_H_
#define Motor_HHS_H_

#include <Zumo32U4.h>
#include <string.h>

/**
 * @class Motor_HHS
 * @brief Klasse voor het aansturen van een motor.
 */
class Motor_HHS {
  public:
      /**
       * @brief Constructor voor de Motor_HHS-klasse.
       * @param motorkant De kant van de motor (bijv. "links" of "rechts").
       * @param pinPWM De PWM-pin voor de motor.
       * @param pinDIR De DIR-pin voor de motor.
       */
      Motor_HHS(String motorkant, int pinPWM, int pinDIR);

      /**
       * @brief Initialiseer de motor.
       */
      void init();

      /**
       * @brief Stel de snelheid van de motor in.
       * @param snelheid De snelheid van de motor (-255 tot 255).
       */
      void setSnelheid(int snelheid);

      /**
       * @brief Haal de huidige snelheid van de motor op.
       * @return De huidige snelheid van de motor.
       */
      int getSnelheid() const;

  private:
    String motorkant; ///< De kant van de motor (bijv. "links" of "rechts").
    int pinPWM; ///< De PWM-pin voor de motor.
    int pinDIR; ///< De DIR-pin voor de motor.
    int huidigesnelheid; ///< De huidige snelheid van de motor.
};

#endif
