#include "Motor.h"

Zumo32U4Encoders encoders;

Motoren::Motoren():status(true), staticSpeed(0) {
}

void Motoren::Stop() {
  Motor.setSpeeds(0, 0);
}

void Motoren::GaVooruit(int speed) {
  SpeedL = speed;
  SpeedR = speed;
  staticSpeed = speed;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::SetStaticSpeed(int speed) {
  SpeedL = speed;
  SpeedR = speed;
  staticSpeed = speed;
}

void Motoren::GaAchteruit(int speed) {
  SpeedL = -speed;
  SpeedR = -speed;
  staticSpeed = -speed;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::GaLinks(int speed) {
  SpeedL = staticSpeed;
  SpeedR = staticSpeed;

  if (SpeedL > 0) {
    SpeedL = staticSpeed - speed;
  } else if (SpeedL < 0) {
    SpeedL = staticSpeed + speed;
  } else {
    SpeedL = -speed;
    SpeedR = speed;
  }

  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::GaRechts(int speed) {
  SpeedL = staticSpeed;
  SpeedR = staticSpeed;

  if (SpeedR > 0) {
    SpeedR = staticSpeed - speed;
  } else if (SpeedR < 0) {
    SpeedR = staticSpeed + speed;
  } else {
    SpeedR = -speed;
    SpeedL = speed;
  }

  Motor.setSpeeds(SpeedL, SpeedR);
}

// Encoder methods implementation
void Motoren::resetEncoders() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

float Motoren::getDistanceTraveled() {
  // Get average encoder counts and convert to distance
  // Zumo32U4 has ~75.81 counts per cm (assuming 120:1 gear ratio and 3.2cm wheel diameter)
  long leftCount = abs(encoders.getCountsLeft());
  long rightCount = abs(encoders.getCountsRight());
  long avgCount = (leftCount + rightCount) / 2;
  return avgCount / 75.81; // Convert to cm
}

long Motoren::getLeftEncoderCount() {
  return encoders.getCountsLeft();
}

long Motoren::getRightEncoderCount() {
  return encoders.getCountsRight();
}
