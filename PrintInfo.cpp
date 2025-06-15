#include "PrintInfo.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "PushBlock.h"

// extern BASE_SPEED;
extern PushBlock pushblock;
extern LineSensor linesensor;
extern Xbee xbee;

PrintInfo::PrintInfo() {}

void PrintInfo::printHelp() {
        Serial1.println(F("Input Key | Action                       | Condition           | Note"));
        Serial1.println(F("----------+------------------------------+---------------------+-----------------------------"));
        Serial1.println(F("W         | Move forward                 | Manual control mode | Case insensitive"));
        Serial1.println(F("S         | Move backward                | Manual control mode | Case insensitive"));
        Serial1.println(F("A         | Turn left                    | Manual control mode | Case insensitive"));
        Serial1.println(F("D         | Turn right                   | Manual control mode | Case insensitive"));
        Serial1.println(F("  or 0    | Stop                         | None                | Blank (eg spacebar)"));
        Serial1.println(F("X         | Print Diagnostics to XBee    | None                | Case insensitive"));
        Serial1.println(F("H         | Print overview of commands   | None                | Case insensitive"));
        Serial1.println(F("P         | Start linefollowing          | None                | Case insensitive"));
        Serial1.println(F("O         | Stop linefollowing & motors  | None                | Case insensitive"));
        Serial1.println(F("----------+------------------------------+---------------------+-----------------------------"));
}

void PrintInfo::printDiagnostic() {
        Serial1.println("--- Zumo Diagnostics ---");
        Serial1.print("Program Running: ");
        Serial1.println(pushblock.automationRunning ? "Yes" : "No");
        Serial1.print("Input Buffer: ");
        Serial1.println(xbee.getLastChar());
        Serial1.print("Linesensor Value: ");
        Serial1.println(linesensor.giveRawValue(2));
        Serial1.println("--- End Diagnostics ---");
}