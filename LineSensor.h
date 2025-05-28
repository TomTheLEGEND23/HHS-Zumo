#include "WString.h"
#include <Zumo32U4.h>
#include "XBee.h"

class LineSensor
{
public:
    LineSensor();
    void calibrateLineSensor();
    String detectedLine(int);
    unsigned int giveRawValue(int);

private:
    const uint16_t blackThreshold = 800;
    const uint16_t grayThresholdtop = 380;
    const uint16_t grayThresholdbottom = 280;
    const uint16_t greenThresholdtop = 180;
    const uint16_t greenThresholdbottom = 140;
    const uint16_t brownThresholdtop = 130;
    const uint16_t brownThresholdbottom = 100;
    unsigned int lineSensorValues[5];
    Zumo32U4LineSensors zumoLineSensor;
    Zumo32U4ButtonA zumoAButton;

};