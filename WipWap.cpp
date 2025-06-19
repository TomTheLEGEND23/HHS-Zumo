#include "WipWap.h"
#include "Motor.h"
#include "IMU.h"

extern IMU imu;
extern Motoren motor;

WipWap::WipWap()
  : completed(false), distance(0) {}

void WipWap::Wippen() {
  motor.ResetEncoder();
  completed = false;
  float pitch = imu.pitch();  // Still use pitch to determine direction
  int delayTime = 500;        // Fixed delay

  if (pitch > 0.1) {
    distance = motor.GetEncoderLeft() - 3;

    motor.SetSpeed(-200);
    motor.Beweeg();

    while (distance < motor.GetEncoderLeft()) {
      // Waiting until encoder reaches target
    }
    motor.Stop();
    delay(delayTime);
  } else if (pitch < -0.1) {
    distance = motor.GetEncoderLeft() + 3;

    motor.SetSpeed(200);
    motor.Beweeg();

    while (distance > motor.GetEncoderLeft()) {
      // Waiting until encoder reaches target
    }
    motor.Stop();
    delay(delayTime);
  }

  if (pitch > -0.1 && pitch < 0.1) {
    int now = millis();
    int interval = millis() + 5000;
    pitch = imu.pitch();  // Re-check pitch to see if it stabilizes

    while (now < interval && pitch > -0.09 && pitch < 0.09) {
      now = millis();
    }
    if (now > interval) {
      completed = true;
      motor.SetSpeed(200);
      motor.Beweeg();
      delay(500);
      motor.Stop();
      return;
    }
  }
}

bool WipWap::completedWippen() {
  return completed;
}
