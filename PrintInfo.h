#ifndef PRINTINFO_H
#define PRINTINFO_H

#include <Arduino.h>
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"

class PrintInfo {
public:
    PrintInfo();

    void printHelp();
    void printDiagnostic();

private:

};

#endif // PRINTINFO_H