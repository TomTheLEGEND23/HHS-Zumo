#include "Motor_HHS.h"
#include <FastGPIO.h>
#include <avr/io.h>

Motor_HHS::Motor_HHS(String M, int pPWM, int pDIR) : motorkant(M), pinPWM(pPWM), pinDIR(pDIR), huidigesnelheid(0) {}

//Wanneer er variablen werden gebruikt voor de pin nummers werkt de FastGPIO niet. Als dit er nog staat dan is daar geen oplossing voor gevonden.
#define PWM_L 10
#define PWM_R 9
#define DIR_L 16
#define DIR_R 15

void Motor_HHS::init() {

    FastGPIO::Pin<PWM_L>::setOutputLow();
    FastGPIO::Pin<PWM_R>::setOutputLow();

    //Pwm wordt ingesteld, pwm werkt met een percentage van de tijd dat het aanstaat.
    // In dit geval is dat tussen de 0 en 400 dus als er 200 speed is staat de duty cycle 50% hoog(en dus ook 50% laag).
    // Hoe langer de duty cycle hoe meer vermogen de motor krijgt en dus hoe sneller hij draait.
    //De timer voor PWM worden ingesteld. Heeft te maken met welke pinnen de PWM beginnen.
    TCCR1A = 0b10100000;
    TCCR1B = 0b00010001;
    ICR1 = 400;
    OCR1A = 0;
    OCR1B = 0;
}

//Met deze functie worden de motoren aangezet.
void Motor_HHS::setSnelheid(int snelheid) {
    bool achteruit = false;

    //Hier wordt de snelheid omgezet in een positieve waarde en een achteruit of vooruit waarde.
    if (snelheid < 0) {
        snelheid = -snelheid;
        achteruit = true;
    }
    //Hier wordt de snelheid gelimit op 400 wat dit is de max PWM dat is ingesteld bij de PWM setup.
    if (snelheid > 400) {
        snelheid = 400;
    }

    huidigesnelheid = snelheid;
    // Hier worden de motoren aangestuurd
    //Wanneer er variablen werden gebruikt voor de pin nummers werkt de FastGPIO niet. Als dit er nog staat dan is daar geen oplossing voor gevonden
    if (motorkant == "rechts") {
        OCR1A = snelheid;
        FastGPIO::Pin<DIR_R>::setOutput(achteruit);
    } else if (motorkant == "links") {
        OCR1B = snelheid;
        FastGPIO::Pin<DIR_L>::setOutput(achteruit);
    }
    
}
// Deze functie geeft de huidige snelheid van de gespecificeerde motor weer
int Motor_HHS::getSnelheid() const {
    return huidigesnelheid;
}
