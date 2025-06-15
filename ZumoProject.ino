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

  if (xbee.isButtonPressed('i')) {
    pushAutomation = true;
  }

  if (xbee.isButtonPressed('u')) {
    WipAutomation = true;
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
    pushblock.pushBlock(300);
    if (pushblock.completedPushing()) pushAutomation = false;
  }
  if (WipAutomation) {
    wipwap.Wippen();
    if (wipwap.completedWippen()) {
      WipAutomation = false;
      automationRunning = true;
      Serial1.println("EndedWipWap");
    }
  }
}