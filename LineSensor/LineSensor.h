#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <Zumo32U4.h>

class LineSensor
{
public:
    LineSensor();
    void init();
    bool isBlackLineDetected();
    void readSensorValues(uint16_t *sensorValues);
    void printSensorValues();

private:
    static const uint8_t NUM_SENSORS = 5;
    static const uint16_t BLACK_LINE_THRESHOLD = 800;
    Zumo32U4LineSensors lineSensors;
};

#endif // LINESENSOR_H