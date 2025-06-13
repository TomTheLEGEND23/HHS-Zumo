#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "WString.h"
#include <Zumo32U4.h>
#include "XBee.h"
#include "Motor.h"

class LineSensor
{
public:
    LineSensor();
    void calibrateLineSensor(Xbee &xbee, Motoren &motors);
    int detectedLine();
    int giveRawValue(int);
    int giveCalValue(int);

private:
    unsigned int linesensorRawValue[5];
    unsigned int MinimumDetection[5] {1023, 1023, 1023, 1023, 1023}; // initialize to max value
    unsigned int MinimumDetection[5] {1023, 1023, 1023, 1023, 1023}; // initialize to max value
    Zumo32U4LineSensors zumoLineSensor;
};

#endif
