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
    windowStartTime(0),
    toggleCount(0),
    lastReading(false),
    pushingObject(false),
    initial(false),
    completed(false) {}

bool PushBlock::completedPushing() {
  return completed;
}

void PushBlock::pushBlock(int BASE_SPEED) {
  int sensorValues[5];
  linesensor.updateSensors(sensorValues);

  if (completed) {
    // Reset state for a new push cycle
    initial = false;
    pushingObject = false;
    completed = false;
  }

  if (!initial) {
    Serial1.println("Positioning before push...");
    distance = motor.GetEncoderLeft() + 1100;
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
    while (distance > motor.GetEncoderLeft()) {}

    motor.rotateLeft90();
    motor.rotateLeft90();

    windowStartTime = millis();
    toggleCount = 0;
    lastReading = false;
    pushingObject = false;
    initial = true;

    Serial1.println("Push sequence started.");
  }

  if (linesensor.detectedLine(0, 5) == -1) {
    // No line detected, continue searching/pushing
    linesensor.updateSensors(sensorValues);

    bool currentReading = proximitysensor.objectDetectedFront();
    unsigned long now = millis();

    if (!pushingObject && initial) {
      if (currentReading != lastReading) {
        toggleCount++;
      }

      if (now - windowStartTime >= toggleInterval) {
        if (toggleCount >= toggleThreshold) {
          pushingObject = true;
          Serial1.println("Object detected. Pushing...");
        }
        windowStartTime = now;
        toggleCount = 0;
      }

      if (currentReading || pushingObject) {
        motor.SetSpeed(BASE_SPEED);
        motor.Beweeg();
      } else {
        motor.turn(180, -180);
        Serial1.println("Searching for object...");
      }
    } else {
      motor.SetSpeed(BASE_SPEED);
      motor.Beweeg();
    }

    lastReading = currentReading;
  } else {
    // Line detected, stop and mark as completed
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