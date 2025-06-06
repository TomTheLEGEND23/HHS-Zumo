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
    
    Serial.print("\tF: ");
    Serial.print(zumoProxSensors.countsFrontWithLeftLeds());
    Serial.print("/");
    Serial.print(zumoProxSensors.countsFrontWithRightLeds());
    
    Serial.print(" F:");
    Serial.print(objectDetectedFront() ? "Y" : "N");
    Serial.println("");
}

bool ProximitySensors::strongObjectDetectedFront() {
    zumoProxSensors.read();
    int leftReading = zumoProxSensors.countsFrontWithLeftLeds();
    int rightReading = zumoProxSensors.countsFrontWithRightLeds();
    
    // Strong detection means both sensors see something significant
    return (leftReading > detectionThreshold * 2) && (rightReading > detectionThreshold * 2);
}

int ProximitySensors::scanForObject() {
    zumoProxSensors.read();
    int leftReading = zumoProxSensors.countsFrontWithLeftLeds();
    int rightReading = zumoProxSensors.countsFrontWithRightLeds();
    int totalReading = leftReading + rightReading;
    
    // Return total reading strength (0 = no object, higher = stronger detection)
    return totalReading;
}
