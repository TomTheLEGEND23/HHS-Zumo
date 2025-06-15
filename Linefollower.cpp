#include "Linefollower.h"
#include <ctype.h>

LineFollower::LineFollower() {
}

void LineFollower::FollowLine(Motoren &motor, LineSensor &linesensor, const int BASE_SPEED, const int MAX_SPEED, const float KP) {
  int sensorValues[5];
  linesensor.updateSensors(sensorValues);
  //-----------------------------------

  // Left sensor (sensorValues[0])
  int currentLeft = sensorValues[0];
  if (currentLeft > previousLeftValue) {
    leftRising = true;
    leftPeak = currentLeft;
  } else if (leftRising && currentLeft < previousLeftValue) {
    // Peak detected
    leftRising = false;
    if (leftPeak >= 390 && leftPeak <= 410) {
      LeftGray = true;
      leftSensor = 1;
      rightSensor = 4;
      Serial1.println("Gray Detected on left");
      Serial1.println(leftPeak);
    } else if (leftPeak > 500) {
      Serial1.println("Black line detected on left — ignoring as gray");
    }
  }
  previousLeftValue = currentLeft;

  // Right sensor (sensorValues[4])
  int currentRight = sensorValues[4];
  if (currentRight > previousRightValue) {
    rightRising = true;
    rightPeak = currentRight;
  } else if (rightRising && currentRight < previousRightValue) {
    // Peak detected
    rightRising = false;
    if (rightPeak >= 390 && rightPeak <= 410) {
      RightGray = true;
      leftSensor = 1;
      rightSensor = 4;
      Serial1.println("Gray Detected on right");
      Serial1.println(rightPeak);
    } else if (rightPeak > 500) {
      Serial1.println("Black line detected on right — ignoring as gray");
    }
  }
  previousRightValue = currentRight;

  //------------------------------------------
  // Check left corner
  if ((sensorValues[0] >= 800) && (sensorValues[4] < 70) && LeftGray) {
    onLeftCorner = true;
  }
  if (onLeftCorner) {
    if (sensorValues[0] < 70) {
      delay(10);
      motor.rotateLeft90();
      onLeftCorner = false;
      leftSensor = 0;
      rightSensor = 5;
      Serial1.println("Gray left rotation");
    }
  }

  // Check right corner
  if ((sensorValues[4] >= 800) && (sensorValues[0] < 70) && RightGray) {
    onRightCorner = true;
  }
  if (onRightCorner) {
    if (sensorValues[4] < 70) {
      delay(10);
      motor.rotateRight90();
      onRightCorner = false;
      leftSensor = 0;
      rightSensor = 5;
      Serial1.println("Gray right rotation");
    }
  }
  //------------------------------------------

  int linePos = linesensor.detectedLine(leftSensor, rightSensor);

  if (linePos == -1) {
    // No line detected — move forward at half speed
    motor.SetSpeed(BASE_SPEED / 2);
    motor.Beweeg();
    return;
  }

  int movementSpeed = 0

    if (sensorValues[0] < 200 && sensorValues[1] < 200 && sensorValues[2] < 200 && sensorValues[3] < 200 && sensorValues[4] < 200) {
    movementSpeed = BASE_SPEED / 2;
  }
  else {
    movementSpeed = BASE_SPEED;
  }

  int error = linePos - 2000;
  int correction = KP * error;

  int leftSpeed = constrain(movementSpeed + correction, -MAX_SPEED, MAX_SPEED);
  int rightSpeed = constrain(movementSpeed - correction, -MAX_SPEED, MAX_SPEED);

  motor.turn(leftSpeed, rightSpeed);
}