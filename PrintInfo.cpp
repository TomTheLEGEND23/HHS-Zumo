#include "PrintInfo.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"

// extern BASE_SPEED;
extern bool automationRunning;
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
        Serial1.println(F("  or 0    | Stop                         | Manual control mode | Blank (eg spacebar)"));
        Serial1.println(F("X         | Print Diagnostics to XBee    | None                | Case insensitive"));
        Serial1.println(F("+ or =    | Increase speed               | Manual control mode | Prints new speed to XBee"));
        Serial1.println(F("- or _    | Decrease speed               | Manual control mode | Prints new speed to XBee"));
        Serial1.println(F("M         | Reset speed                  | Manual control mode | Prints new speed to XBee"));
        Serial1.println(F("H         | Print overview of commands   | None                | Case insensitive"));
        Serial1.println(F("P         | Start linefollowing          | None                | Case insensitive"));
        Serial1.println(F("O         | Stop linefollowing           | None                | Case insensitive"));
        Serial1.println(F("----------+------------------------------+---------------------+-----------------------------"));
}

void PrintInfo::printDiagnostic() {
        Serial1.println("--- Zumo Diagnostics ---");
        Serial1.print("Program Running: ");
        Serial1.println(automationRunning ? "Yes" : "No");
        Serial1.print("Input Buffer: ");
        Serial1.println(xbee.getLastChar());
        Serial1.println("Linesensor Value: ");
        Serial1.println(linesensor.detectedLine());
        Serial1.println("--- End Diagnostics ---");
}