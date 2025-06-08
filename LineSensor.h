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
    unsigned int giveRawValue(int index);

private:
private:
    unsigned int linesensorRawValue[5];
    Zumo32U4LineSensors zumoLineSensor;
};

#endif
