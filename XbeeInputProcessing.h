#ifndef XBEEINPUTPROCESSING_H
#define XBEEINPUTPROCESSING_H

#include <Arduino.h>

class XbeeInputProcessing {
public:
    XbeeInputProcessing();
    void processKeyPressRun();
    void processKeyPressMan();
    void processKeyInput(char ReChar);
    void printHelp();

private:
    int MAN_SPEED;
    int MAN_TURN_SPEED;
    String inputString;
};

#endif