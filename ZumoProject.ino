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

// State flags for gray → white → black pattern detection (LEFT)
bool leftGraySeen = false;
bool leftWhiteSeen = false;
bool leftPrevWhite = true;  // Assume starting on white
unsigned long leftPatternStartTime = 0;

// State flags for gray → white → black pattern detection (RIGHT)
bool rightGraySeen = false;
bool rightWhiteSeen = false;
bool rightPrevWhite = true;  // Assume starting on white
unsigned long rightPatternStartTime = 0;

#define BASE_SPEED 200
#define MAX_SPEED 400
#define KP 0.5

// Sensor thresholds (adjust based on your surface and tests)
const int grayThreshold = 100;    // Values below are white
const int blackThreshold = 650;   // Values above are black

const unsigned long patternTimeout = 3000;  // 3 seconds timeout for pattern

void setup() {
}

void loop() {
  xbee.update();

  // Start calibration
  if (xbee.isButtonPressed('c')) {
    imu.init();
    linesensor.calibrateLineSensor(xbee, motor);
  }

  // Start automation
  if (xbee.isButtonPressed('p')) {
    automationRunning = true;
  }

  // Stop automation
  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    motor.Stop();
  }

  if (automationRunning) {
    int linePos = linesensor.detectedLine();

    int s0Cal = linesensor.giveCalValue(0);  // Left sensor
    int s4Cal = linesensor.giveCalValue(4);  // Right sensor

    unsigned long currentTime = millis();

    // --- LEFT SENSOR (S0) pattern detection ---
    // Update prevWhite based on current reading
    if (s0Cal < grayThreshold) {
      leftPrevWhite = true;
    }

    if (s0Cal > grayThreshold && s0Cal < blackThreshold) {
      // Only start gray detection if previously white
      if (leftPrevWhite && !leftGraySeen) {
        leftPatternStartTime = currentTime;
        Serial1.println("LEFT: Gray detected (after white)");
        leftGraySeen = true;
        leftPrevWhite = false;  // now gray, no longer white
      }
    }

    if (leftGraySeen && s0Cal < grayThreshold) {
      if (!leftWhiteSeen) Serial1.println("LEFT: White detected after gray");
      leftWhiteSeen = true;
    }

    // Reset if gray detected and immediately black without white
    if (leftGraySeen && !leftWhiteSeen && s0Cal >= blackThreshold) {
      Serial1.println("LEFT: Black detected directly after gray without white — resetting pattern");
      leftGraySeen = false;
      leftWhiteSeen = false;
      leftPrevWhite = false;
    }

    // Execute left turn if pattern completed
    if (leftGraySeen && leftWhiteSeen && s0Cal >= blackThreshold) {
      Serial1.println("LEFT: Complete pattern detected — rotating left");
      motor.Stop();
      delay(200);
      motor.rotateLeft90();
      leftGraySeen = false;
      leftWhiteSeen = false;
      leftPrevWhite = false;
      return;
    }

    // Timeout reset for LEFT
    if (leftGraySeen && (currentTime - leftPatternStartTime > patternTimeout)) {
      Serial1.println("LEFT: Pattern timeout — resetting");
      leftGraySeen = false;
      leftWhiteSeen = false;
      leftPrevWhite = false;
    }

    // --- RIGHT SENSOR (S4) pattern detection ---
    // Update prevWhite based on current reading
    if (s4Cal < grayThreshold) {
      rightPrevWhite = true;
    }

    if (s4Cal > grayThreshold && s4Cal < blackThreshold) {
      // Only start gray detection if previously white
      if (rightPrevWhite && !rightGraySeen) {
        rightPatternStartTime = currentTime;
        Serial1.println("RIGHT: Gray detected (after white)");
        rightGraySeen = true;
        rightPrevWhite = false;  // now gray, no longer white
      }
    }

    if (rightGraySeen && s4Cal < grayThreshold) {
      if (!rightWhiteSeen) Serial1.println("RIGHT: White detected after gray");
      rightWhiteSeen = true;
    }

    // Reset if gray detected and immediately black without white
    if (rightGraySeen && !rightWhiteSeen && s4Cal >= blackThreshold) {
      Serial1.println("RIGHT: Black detected directly after gray without white — resetting pattern");
      rightGraySeen = false;
      rightWhiteSeen = false;
      rightPrevWhite = false;
    }

    // Execute right turn if pattern completed
    if (rightGraySeen && rightWhiteSeen && s4Cal >= blackThreshold) {
      Serial1.println("RIGHT: Complete pattern detected — rotating right");
      motor.Stop();
      delay(200);
      motor.rotateRight90();
      rightGraySeen = false;
      rightWhiteSeen = false;
      rightPrevWhite = false;
      return;
    }

    // Timeout reset for RIGHT
    if (rightGraySeen && (currentTime - rightPatternStartTime > patternTimeout)) {
      Serial1.println("RIGHT: Pattern timeout — resetting");
      rightGraySeen = false;
      rightWhiteSeen = false;
      rightPrevWhite = false;
    }

    // --- Normal line following ---
    if (linePos == -1) {
      motor.SetSpeed(BASE_SPEED / 2);
      motor.Beweeg();
      return;
    }

    int error = linePos - 2000;
    int correction = KP * error;
    int leftSpeed = BASE_SPEED + correction;
    int rightSpeed = BASE_SPEED - correction;

    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

    motor.turn(leftSpeed, rightSpeed);
  }
}
