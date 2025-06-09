#include "XbeeInputProcessing.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"

extern Xbee xbee;
extern Motoren motor;
extern LineSensor linesensor;
int MAN_SPEED = 300;
int MAN_TURN_SPEED = MAN_SPEED / 2;
extern bool automationRunning;
extern bool lineSensorCalibration;

XbeeInputProcessing::XbeeInputProcessing(){}

void XbeeInputProcessing::processKeyInput(char ReChar) {
    // Serial1.println(ReChar);
    inputString = ReChar;
    if (!automationRunning) {
        processKeyPressMan();
    } else if (automationRunning) {
        processKeyPressRun();
    }
}

void XbeeInputProcessing::processKeyPressMan() {
    char command = inputString[0];
    // Serial1.println("Manual Control Enabled");
    // Serial1.println(command);
    if ((command == 'W') || (command == 'w')) {
        Serial1.println("Moving Forward");
        motor.SetSpeed(MAN_SPEED);
        motor.Beweeg();
    }
    else if ((command == 'A') || (command == 'a')) {
        Serial1.println("Turning left");
        motor.turn(MAN_TURN_SPEED*-1, MAN_TURN_SPEED);
        //motor.Beweeg();
    }
    else if ((command == 'S') || (command == 's')) {
        Serial1.println("Moving Back");
        motor.SetSpeed(MAN_SPEED*-1);
        motor.Beweeg();
    }
    else if ((command == 'D') || (command == 'd')) {
        Serial1.println("Turning right");
        motor.turn(MAN_TURN_SPEED, MAN_TURN_SPEED*-1);
        //motor.Beweeg();
    }
    else if ((command == ' ') || (command == 0)) {
        motor.Stop();
    }
    else if ((command == 'X') || (command == 'x')) {
        Serial1.println("--- Zumo Diagnostics ---");
        Serial1.print("Program Running: ");
        Serial1.println(automationRunning ? "Yes" : "No");
        Serial1.print("Input Buffer: ");
        Serial1.println(xbee.getLastChar());
        Serial1.println("Linesensor Value: ");
        Serial1.println(linesensor.detectedLine());
        Serial1.println("Motor Speed (Manual Control)");
        Serial1.println(MAN_SPEED);
        Serial1.println("--- End Diagnostics ---");
    }
    else if ((command == '+') || (command == '=')) {
        MAN_SPEED = MAN_SPEED + 50;
        Serial1.print("Increased Motor Speed. It's now ");
        Serial1.println(MAN_SPEED);
    }
    else if ((command == '-') || (command == '_')) {
        Serial1.print("Decreased Motor Speed. It's now ");
        Serial1.println(MAN_SPEED);
        Serial1.println("Decreased Motor Speed. It's now" + MAN_SPEED);
    }
    else if ((command == 'P') || (command == 'p')) {
        Serial1.println("Program running");
        automationRunning = true;
    }
    else if ((command == 'H') || (command == 'h')) {
        printHelp();
    }
    else if ((command == 'C') || (command == 'c')) {
        lineSensorCalibration = true;
    }
}

void XbeeInputProcessing::processKeyPressRun() {
    char command = inputString[0];
    // Serial1.println("Manual Control Disabled");
    // Serial1.println(command);
    if ((command == 'W') || (command == 'w') || (command == 'A') || (command == 'a') || (command == 'S') || (command == 's') || (command == 'D') || (command == 'd') || (command == ' ') || (command == 0)) {
        Serial1.println("Move command not allowed while not in Manual control mode. Disable linefollowing to access manual control commands.");
    }
    else if ((command == 'X') || (command == 'x')) {
        Serial1.println("--- Zumo Diagnostics ---");
        Serial1.print("Program Running: ");
        Serial1.println(automationRunning ? "Yes" : "No");
        Serial1.println("Linesensor Value: ");
        Serial1.println(linesensor.detectedLine());
        Serial1.println("--- End Diagnostics ---");
    }
    else if ((command == 'O') || (command == 'o')) {
        automationRunning = false;
        Serial1.println("Program stopped");
        motor.Stop();
    }
    else if ((command == 'H') || (command == 'h')) {
        printHelp();
    }
}

void XbeeInputProcessing::printHelp() {
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