#include "ProximitySensors.h"

ProximitySensors::ProximitySensors() {
    zumoProxSensors.initFrontSensor();
}

bool ProximitySensors::objectDetectedFront() {
    zumoProxSensors.read();
    return (zumoProxSensors.countsFrontWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsFrontWithRightLeds() > detectionThreshold);
}

void ProximitySensors::printReadings() {
    zumoProxSensors.read();
    
    Serial1.print("\tF: ");
    Serial1.print(zumoProxSensors.countsFrontWithLeftLeds());
    Serial1.print("/");
    Serial1.print(zumoProxSensors.countsFrontWithRightLeds());
    
    Serial1.print(" F:");
    Serial1.print(objectDetectedFront() ? "Y" : "N");
    Serial1.println("");
}

int ProximitySensors::countsFrontAvg() {
    zumoProxSensors.read();
    int FL = zumoProxSensors.countsFrontWithLeftLeds();
    int FR = zumoProxSensors.countsFrontWithRightLeds();
    int avg = (FL + FR)/2;
    return avg;
}

int ProximitySensors::countsFrontLeft() {
    zumoProxSensors.read();
    int FL = zumoProxSensors.countsFrontWithLeftLeds();
    return FL;
}

int ProximitySensors::countsFrontRight() {
    zumoProxSensors.read();
    int FR = zumoProxSensors.countsFrontWithRightLeds();
    return FR;
}