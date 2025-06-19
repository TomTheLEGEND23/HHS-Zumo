#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "Linefollower.h"
#include "PushBlock.h"
#include "WipWap.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
Xbee xbee;
LineFollower linefollower;
PushBlock pushblock;
WipWap wipwap;

bool automationRunning = false;
bool pushAutomation = false;
bool WipAutomation = false;

const int BASE_SPEED = 180;
const int MAX_SPEED = 400;
const float KP = 0.5;

void setup() {
}

void loop() {
  xbee.update();

  // Start calibration
  if (xbee.isButtonPressed('c')) {
    imu.init();
    linesensor.calibrateLineSensor(xbee, motor);
  }

  // Start line following
  if (xbee.isButtonPressed('p')) automationRunning = true;

  if (linefollower.GetLeftGray() && linefollower.GetRightGray()) {
    WipAutomation = true;
    automationRunning = false;
  }

  if (linefollower.GetLeftBrown() && linefollower.GetRightBrown()) {
    pushAutomation = true;
    automationRunning = false;
  }

  // Stop line following
  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    pushAutomation = false;
    WipAutomation = false;
    motor.Stop();
  }

  if (automationRunning) {
    linefollower.FollowLine(motor, linesensor, BASE_SPEED, MAX_SPEED, KP);
  }
  if (pushAutomation) {
    pushblock.pushBlock(250);
    if (pushblock.completedPushing()) {
      pushAutomation = false;
      linefollower.SetLeftBrown(0);
      linefollower.SetRightBrown(0);
    }
  }

  if (WipAutomation) {
    wipwap.Wippen();
    if (wipwap.completedWippen()) {
      if (wipwap.completedWippen()) {
        WipAutomation = false;
        linefollower.SetLeftGray(0);
        linefollower.SetRightGray(0);
        automationRunning = true;
      }
    }
  }
}