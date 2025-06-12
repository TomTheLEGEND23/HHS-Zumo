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

bool automationRunning = false;
bool onLeftCorner = false;
bool onRightCorner = false;

// Control parameters
#define BASE_SPEED 200
#define MAX_SPEED 400
#define KP 0.5

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
  if (xbee.isButtonPressed('p')) {
    automationRunning = true;
  }

  // Stop line following
  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    motor.Stop();
  }


  int sensorValues[5];
  linesensor.updateSensors(sensorValues);
  // Serial1.println(sensorValues[0]);

  // Line following logic
  if (automationRunning) {

    // Check left corner
    if ((sensorValues[0] >= 200 && sensorValues[0] <= 300) || sensorValues[0] >= 700) {
      if (sensorValues[4] < 70) {
        onLeftCorner = true;
      }
    }
    if (onLeftCorner) {
      if (sensorValues[0] < 70) {
        motor.rotateLeft90();
        onLeftCorner = false;
      } else if (sensorValues[0] > 300 && sensorValues[0] < 700) {
        // Exit if gray
        onLeftCorner = false;
      }
    }

    // Check right corner
    if ((sensorValues[4] >= 200 && sensorValues[4] <= 300) || sensorValues[4] >= 700) {
      if (sensorValues[0] < 70) {
        onRightCorner = true;
      }
    }
    if (onRightCorner) {
      if (sensorValues[4] < 70) {
        motor.rotateRight90();
        onRightCorner = false;
      } else if (sensorValues[4] > 300 && sensorValues[4] < 700) {
        // Exit if gray
        onRightCorner = false;
      }
    }

    int linePos = linesensor.detectedLine();

    if (linePos == -1) {
      // No line detected — move forward at half speed
      motor.SetSpeed(BASE_SPEED / 2);
      motor.Beweeg();
      return;
    }

    int movementSpeed = BASE_SPEED;

    // Calculate error from center (2000)
    int error = linePos - 2000;

    // Proportional correction
    int correction = KP * error;

    // Compute motor speeds
    int leftSpeed = movementSpeed + correction;
    int rightSpeed = movementSpeed - correction;

    // Constrain speeds
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

    // Drive using your Motoren class
    motor.turn(leftSpeed, rightSpeed);
  }
}