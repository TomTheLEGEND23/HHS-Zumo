#include "WipWap.h"
#include "Motor.h"
#include "IMU.h"

extern IMU imu;
extern Motoren motor;

WipWap::WipWap()
  : completed(false), distance(0) {}

void WipWap::Wippen() {
  completed = false;
  float pitch = imu.pitch();
  float absPitch = fabs(pitch);
  float maxPitch = 0.3;
  int maxDelay = 500;
  int minDelay = 100;

  if (absPitch > maxPitch) absPitch = maxPitch;

  int delayTime = maxDelay - (int)((absPitch / maxPitch) * (maxDelay - minDelay));
  int interval = millis() + 5000;

  Serial1.print("Initial pitch: ");
  Serial1.println(pitch);
  Serial1.print("Absolute pitch: ");
  Serial1.println(absPitch);
  Serial1.print("Calculated delayTime (ms): ");
  Serial1.println(delayTime);

  if (pitch > 0.03) {
    distance = motor.GetEncoderLeft() - 3;
    Serial1.print("Tilting Forward. Target Encoder: ");
    Serial1.println(distance);

    motor.SetSpeed(-200);
    motor.Beweeg();

    while (distance < motor.GetEncoderLeft()) {
      Serial1.print("Encoder value: ");
      Serial1.println(motor.GetEncoderLeft());
    }
    motor.Stop();
    Serial1.println("Stopped after forward movement.");
    delay(delayTime);
  } else if (pitch < -0.03) {
    distance = motor.GetEncoderLeft() + 3;
    Serial1.print("Tilting Backward. Target Encoder: ");
    Serial1.println(distance);

    motor.SetSpeed(200);
    motor.Beweeg();

    while (distance > motor.GetEncoderLeft()) {
      Serial1.print("Encoder value: ");
      Serial1.println(motor.GetEncoderLeft());
    }
    motor.Stop();
    Serial1.println("Stopped after backward movement.");
    delay(delayTime);
  } else {
    Serial1.println("Pitch in neutral range. No tilt movement.");
  }

  while (pitch > -0.03 && pitch < 0.03) {
    int now = millis();
    pitch = imu.pitch();
    Serial1.print("Stabilizing... Pitch: ");
    Serial1.print(pitch);
    Serial1.print(" | Time left (ms): ");
    Serial1.println(interval - now);

    if (now > interval) {
      completed = true;
      Serial1.println("Wippen complete.");
      return;
    }
  }

  Serial1.println("Wippen ended due to pitch out of range.");
}

bool WipWap::completedWippen() {
  return completed;
}
