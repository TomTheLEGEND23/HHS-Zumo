#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
Xbee xbee;

bool moving = false;
bool automationRunning = false;
int motorspeed = 300;
int halfspeed = 

int previousLinePosition = 2000;
unsigned long lastHighTime = 0;
bool AllWhite = false;

void setup() {
}

void loop() {
  xbee.update();

  if (xbee.isButtonPressed('c')) {
    imu.init();
    linesensor.calibrateLineSensor(xbee);
  }

  if (xbee.isButtonPressed('p')) {
    automationRunning = true;
    moving = false;
  }

  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    motor.Stop();
    moving = false;
  }

  if (automationRunning) {
    int linePosition = linesensor.readLine();  // 0 to 4000
    bool centerSeesLine = linesensor.detectedLine(2) == "Black";
    static int aggressiveness = 80;  // Base steering power
    unsigned long currentTime = millis();

    if (!moving) {
      motor.GaVooruit(motorspeed);
      moving = true;
    }

    // --- Adaptive Aggressiveness ---
    if (!centerSeesLine) {
      aggressiveness += 40;  // Increase correction force
      if (aggressiveness > 400) aggressiveness = 400;
    } else {
      aggressiveness = 80;  // Reset when center sees the line
    }

    // --- Forward or Line Following ---
    if (linePosition == 4000 || linePosition == 0) {
      for (int i = 0; i < 5; i++) {
        if (linesensor.detectedLine(i) != "White") {
          AllWhite = false;
          break;  // No need to keep checking
        }
        Serial1.println("forloop");
      }
      AllWhite = true;
    }
    if (AllWhite) {
      motor.GaVooruit(motorspeed);
    } else if (linePosition > 2000 && !AllWhite) {
      int rightPower = map(linePosition, 2000, 4000, aggressiveness / 2, aggressiveness);
      motor.GaRechts(rightPower);
    } else if (linePosition < 2000 && !AllWhite) {
      int leftPower = map(linePosition, 0, 2000, aggressiveness / 2, aggressiveness);
      motor.GaLinks(leftPower);
    }
  }
}