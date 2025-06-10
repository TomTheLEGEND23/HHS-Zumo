#ifndef XBEEINPUTPROCESSING_H
#define XBEEINPUTPROCESSING_H

#include <Arduino.h>
#include "XBee.h"
#include "Motor.h"

class XbeeInputProcessing {
public:
    XbeeInputProcessing();
    void processKeyPressRun(Xbee &xbee, Motoren &motors);
    void processKeyPressMan(Xbee &xbee, Motoren &motors);
    void processKeyInput(char ReChar, Xbee &xbee, Motoren &motors);
    void printHelp();

private:
    int MAN_SPEED;
    int MAN_TURN_SPEED;
    String inputString;
};

#endif