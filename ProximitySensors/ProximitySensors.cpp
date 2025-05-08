#include "ProximitySensors.h"

ProximitySensors::ProximitySensors() {
    // Default constructor
}

ProximitySensors::~ProximitySensors() {
    // Destructor - nothing to clean up
}

void ProximitySensors::initSensors() {
    zumoProxSensors.initThreeSensors();
}

void ProximitySensors::read() {
    zumoProxSensors.read();
}

bool ProximitySensors::objectDetectedLeft() {
    return (zumoProxSensors.countsLeftWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsLeftWithRightLeds() > detectionThreshold);
}

bool ProximitySensors::objectDetectedRight() {
    return (zumoProxSensors.countsRightWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsRightWithRightLeds() > detectionThreshold);
}

bool ProximitySensors::objectDetectedFront() {
    return (zumoProxSensors.countsFrontWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsFrontWithRightLeds() > detectionThreshold);
}

void ProximitySensors::setDetectionThreshold(int threshold) {
    detectionThreshold = threshold;
}

int ProximitySensors::getDetectionThreshold() {
    return detectionThreshold;
}

void ProximitySensors::printReadings() {
    Serial.print("L: ");
    Serial.print(zumoProxSensors.countsLeftWithLeftLeds());
    Serial.print("/");
    Serial.print(zumoProxSensors.countsLeftWithRightLeds());
    
    Serial.print("\tF: ");
    Serial.print(zumoProxSensors.countsFrontWithLeftLeds());
    Serial.print("/");
    Serial.print(zumoProxSensors.countsFrontWithRightLeds());
    
    Serial.print("\tR: ");
    Serial.print(zumoProxSensors.countsRightWithLeftLeds());
    Serial.print("/");
    Serial.println(zumoProxSensors.countsRightWithRightLeds());
    
    Serial.print("Detection: L:");
    Serial.print(objectDetectedLeft() ? "Y" : "N");
    Serial.print(" F:");
    Serial.print(objectDetectedFront() ? "Y" : "N");
    Serial.print(" R:");
    Serial.println(objectDetectedRight() ? "Y" : "N");
}