#include "Arduino.h"
#include "PushBlock.h"
#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"

extern IMU imu;
extern Motoren motor;
extern LineSensor linesensor;
extern ProximitySensors proximitysensor;
extern Xbee xbee;

PushBlock::PushBlock()
  : distance(0),
    pushingObject(false),
    initial(false),
    completed(false) {}

bool PushBlock::completedPushing() {
  return completed;
}

void PushBlock::pushBlock(int BASE_SPEED) {
  motor.ResetEncoder();
  int sensorValues[5];
  linesensor.updateSensors(sensorValues);

  if (completed) {
    // Reset state
    initial = false;
    pushingObject = false;
    completed = false;
    rotatingToFind = true;
    foundObject = false;
    rotatedBack = false;
    readyToPush = false;
  }

  if (!initial) {
    Serial1.println("Positioning before push...");
    distance = motor.GetEncoderLeft() + 1800;
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
    while (distance > motor.GetEncoderLeft()) {}

    motor.rotateLeft90();
    motor.rotateLeft90();

    initial = true;
    Serial1.println("Push sequence started.");
  }

  linesensor.updateSensors(sensorValues);

  if (sensorValues[0] <= 150 || sensorValues[1] <= 150 || sensorValues[2] <= 150 ||
      sensorValues[3] <= 150 || sensorValues[4] <= 150) {

    bool objectDetected = proximitysensor.objectDetectedFront();

    // Phase 1: Rotate until object is seen and lost
    if (rotatingToFind) {
      if (objectDetected && !foundObject) {
        seenStart = millis();
        foundObject = true;
        Serial1.println("Object detected — tracking...");
      } else if (!objectDetected && foundObject) {
        seenEnd = millis();
        rotatingToFind = false;
        Serial1.println("Object lost — will rotate back.");
      } else {
        motor.turn(240, -240); // keep rotating
      }
    }

    // Phase 2: Rotate back for half the time
    if (!rotatingToFind && !rotatedBack) {
      unsigned long objectSeenDuration = seenEnd - seenStart;
      unsigned long rotateBackTime = objectSeenDuration / 2;

      Serial1.print("Rotating back for ");
      Serial1.print(rotateBackTime);
      Serial1.println(" ms");

      motor.turn(-240, 240); // opposite direction
      delay(rotateBackTime);
      motor.Stop();
      rotatedBack = true;
      readyToPush = true;
      Serial1.println("Ready to push.");
    }

    // Phase 3: Start pushing
    if (readyToPush) {
      motor.SetSpeed(BASE_SPEED);
      motor.Beweeg();
      pushingObject = true;
    }

  } else {
    // Line detected — stop
    Serial1.println("Line detected. Reversing...");
    distance = motor.GetEncoderLeft() - 1100;
    motor.SetSpeed(-BASE_SPEED);
    motor.Beweeg();
    while (distance < motor.GetEncoderLeft()) {}
    motor.Stop();
    completed = true;
    Serial1.println("Push sequence complete.");
  }
}

