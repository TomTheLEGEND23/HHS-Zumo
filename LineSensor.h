#include <stdint.h>
#include "WString.h"
#include <Zumo32U4.h>
#include "XBee.h"

class LineSensor
{
public:
    LineSensor();
    void calibrateLineSensor(XBee &xbee);
    String detectedLine(int);
    unsigned int giveRawValue(int);
    int readLine();

private:
    uint16_t blackThreshold[5];
    uint16_t greenThreshold[5];
    unsigned int lineSensorValues[5];
    unsigned int linesensorRawValue[5];
    Zumo32U4LineSensors zumoLineSensor;
};