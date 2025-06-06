#include <Arduino.h>
#include <Zumo32U4.h>

class ProximitySensors {
private:
    Zumo32U4ProximitySensors zumoProxSensors;
    int detectionThreshold = 4;

public:
    ProximitySensors();
    bool objectDetectedFront();
    bool strongObjectDetectedFront();
    int scanForObject();
    void printReadings();
};
