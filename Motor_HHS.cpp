#include "Motor_HHS.h"
#include <FastGPIO.h>
#include <avr/io.h>

/**
 * @brief Constructor voor de Motor_HHS-klasse.
 * @param M De kant van de motor (bijv. "links" of "rechts").
 * @param pPWM De PWM-pin voor de motor.
 * @param pDIR De DIR-pin voor de motor.
 */
Motor_HHS::Motor_HHS(String M, int pPWM, int pDIR) : motorkant(M), pinPWM(pPWM), pinDIR(pDIR), huidigesnelheid(0) {}

#define PWM_L 10 ///< PWM-pin voor de linker motor.
#define PWM_R 9 ///< PWM-pin voor de rechter motor.
#define DIR_L 16 ///< DIR-pin voor de linker motor.
#define DIR_R 15 ///< DIR-pin voor de rechter motor.

/**
 * @brief Initialiseer de motoren en stel de PWM-timers in.
 */
void Motor_HHS::init() {
    FastGPIO::Pin<PWM_L>::setOutputLow();
    FastGPIO::Pin<PWM_R>::setOutputLow();

    // PWM wordt ingesteld. De duty cycle bepaalt het vermogen van de motor.
    TCCR1A = 0b10100000;
    TCCR1B = 0b00010001;
    ICR1 = 400;
    OCR1A = 0;
    OCR1B = 0;
}

/**
 * @brief Stel de snelheid van de motor in.
 * @param snelheid De gewenste snelheid van de motor (-400 tot 400).
 */
void Motor_HHS::setSnelheid(int snelheid) {
    bool achteruit = false;

    // Controleer of de snelheid negatief is (achteruit).
    if (snelheid < 0) {
        snelheid = -snelheid;
        achteruit = true;
    }
    if (snelheid > 400) {
        snelheid = 400; ///< Beperk de snelheid tot het maximum van 400.
    }

    huidigesnelheid = snelheid;

    // Stel de snelheid en richting in op basis van de motorkant.
    if (motorkant == "rechts") {
        OCR1A = snelheid;
        FastGPIO::Pin<DIR_R>::setOutput(achteruit);
    } else if (motorkant == "links") {
        OCR1B = snelheid;
        FastGPIO::Pin<DIR_L>::setOutput(achteruit);
    }
}

/**
 * @brief Haal de huidige snelheid van de motor op.
 * @return De huidige snelheid van de motor.
 */
int Motor_HHS::getSnelheid() const {
    return huidigesnelheid;
}
