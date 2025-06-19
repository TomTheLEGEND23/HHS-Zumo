#include "Linefollower.h"
#include <ctype.h>

LineFollower::LineFollower() {
}

bool LineFollower::GetLeftGray() {
  return LeftGray;
}

bool LineFollower::GetRightGray() {
  return RightGray;
}

void LineFollower::SetLeftGray(int i) {
  LeftGray = i;
}

void LineFollower::SetRightGray(int i) {
  RightGray = i;
}

bool LineFollower::GetLeftBrown() {
  return LeftBrown;
}

bool LineFollower::GetRightBrown() {
  return RightBrown;
}

void LineFollower::SetLeftBrown(int i) {
  LeftBrown = i;
}

void LineFollower::SetRightBrown(int i) {
  RightGray = i;
}

void LineFollower::FollowLine(Motoren &motor, LineSensor &linesensor, const int BASE_SPEED, const int MAX_SPEED, const float KP) {
  int sensorValues[5];
  linesensor.updateSensors(sensorValues);  // Read values from the 5 line sensors
  unsigned long currentTime = millis();

  // === Left Gray Detection Logic ===
  if (sensorValues[0] > 80) {
    if (sensorValues[0] > leftpeakValue) {
      leftpeakValue = sensorValues[0];
      leftedgeDetection = true;
    }
  } else if (leftedgeDetection) {
    leftedgeDetection = false;
    leftlastPeak = leftpeakValue;
    leftpeakValue = 0;
    leftlastPeakTime = currentTime;
  }

  // Invalidate old peak after 1.5 seconds
  if (leftlastPeak != 0 && (currentTime - leftlastPeakTime > 2000)) {
    leftlastPeak = 0;
    LeftGray = false;
    LeftBrown = false;
  }

  // === Right Gray Detection Logic ===
  if (sensorValues[4] > 80) {
    if (sensorValues[4] > rightpeakValue) {
      rightpeakValue = sensorValues[4];
      rightedgeDetection = true;
    }
  } else if (rightedgeDetection) {
    rightedgeDetection = false;
    rightlastPeak = rightpeakValue;
    rightpeakValue = 0;
    rightlastPeakTime = currentTime;
  }
  // Invalidate old peak after 1.5 seconds
  if (rightlastPeak != 0 && (currentTime - rightlastPeakTime > 2000)) {
    rightlastPeak = 0;
    RightGray = false;
    RightBrown = false;
  }

  // === Gray surface detection based on sensor peak ranges ===
  if (leftlastPeak > 380 && leftlastPeak < 520) {
    LeftGray = true;
  }

  if (rightlastPeak > 380 && rightlastPeak < 520) {
    RightGray = true;
  }
  // === Brown surface detection based on sensor peak ranges ===
  if (leftlastPeak > 690 && leftlastPeak < 850) {
    LeftBrown = true;
  }

  if (rightlastPeak > 590 && rightlastPeak < 720) {
    RightBrown = true;
  }

  if (RightBrown && !LeftBrown) { RightBrown = false; }
  if (!RightBrown && LeftBrown) { LeftBrown = false; }

  // === Handle Gray + Black Line = Turn Left ===
  if (leftlastPeak > 1000 && LeftGray) {
    motor.rotateLeft90();
    LeftGray = false;
    leftlastPeak = 0;
  }
  motor.Stop();

  // === Handle Gray + Black Line = Turn Right ===
  if (rightlastPeak > 1000 && RightGray) {
    motor.rotateRight90();
    RightGray = false;
    rightlastPeak = 0;
  }
  motor.Stop();

  // === Corner Detection ===
  if (sensorValues[0] > 200 && !onLeftCorner) {
    onLeftCorner = true;
    leftCornerTime = currentTime;
  }

  if (sensorValues[4] > 200 && !onRightCorner) {
    onRightCorner = true;
    rightCornerTime = currentTime;
  }

  // Reset corner flags after 2 seconds
  if (onLeftCorner && currentTime - leftCornerTime > 1500) {
    onLeftCorner = false;
  }

  if (onRightCorner && currentTime - rightCornerTime > 1500) {
    onRightCorner = false;
  }
  //Reset if both are true which indicates a false reading or intersecion of gray
  if (onLeftCorner && onRightCorner && !LeftGray && !RightGray) {
    leftSensor = 0;
    rightSensor = 5;
  } else {
    leftSensor = 1;
    rightSensor = 4;
  }

  // === Line Following Logic ===
  int linePos = linesensor.detectedLine(leftSensor, rightSensor);

  if (linePos == -1) {
    // Line lost: try to recover based on last detected corner
    if (onLeftCorner && !onRightCorner && !LeftGray && !RightGray) {
      motor.turn(-200, 200);  // Turn left to find the line
      while (linesensor.detectedLine(leftSensor, rightSensor) == -1) {
        linesensor.updateSensors(sensorValues);
      }
      motor.Stop();
      onLeftCorner = false;
      onRightCorner = false;
      LeftGray = false;
    } else if (onRightCorner && !onLeftCorner && !LeftGray && !RightGray) {
      motor.turn(200, -200);  // Turn right to find the line
      while (linesensor.detectedLine(leftSensor, rightSensor) == -1) {
        linesensor.updateSensors(sensorValues);
      }
      motor.Stop();
      onRightCorner = false;
      onLeftCorner = false;
      RightGray = false;
    } else {
      // No corner detected: slow forward search
      motor.SetSpeed(BASE_SPEED / 2);
      motor.Beweeg();
    }
    return;
  }

  // === P Control ===

  // Adjust movement speed based on sensor signal strength
  int movementSpeed = 0;
  if (sensorValues[0] < 200 && sensorValues[1] < 200 && sensorValues[2] < 200 && sensorValues[3] < 200 && sensorValues[4] < 200) {
    movementSpeed = 140;  // Low reflectivity, slow down
  } else {
    movementSpeed = BASE_SPEED;
  }

  // Calculate position error and proportional correction
  int error = linePos - 2000;
  int correction = KP * error;

  // Apply speed correction to left and right motors
  int leftSpeed = constrain(movementSpeed + correction, -MAX_SPEED, MAX_SPEED);
  int rightSpeed = constrain(movementSpeed - correction, -MAX_SPEED, MAX_SPEED);

  motor.turn(leftSpeed, rightSpeed);
}