#ifndef PROXIMITY_SENSORS_H
#define PROXIMITY_SENSORS_H

#include <Arduino.h>
#include <Zumo32U4.h>

class ProximitySensors {
private:
    // Underlying Zumo32U4ProximitySensors object
    Zumo32U4ProximitySensors zumoProxSensors;
    
    // Threshold value for detection
    int detectionThreshold = 5;

public:
    // Constructor/destructor
    ProximitySensors();
    ~ProximitySensors();
    
    // Basic methods
    void initSensors();
    void read();
    
    // Detection methods
    bool objectDetectedLeft();
    bool objectDetectedRight();
    bool objectDetectedFront();
    
    // Configuration
    void setDetectionThreshold(int threshold);
    int getDetectionThreshold();
    
    // Utility methods
    void printReadings();
};

#endif // PROXIMITY_SENSORS_H

