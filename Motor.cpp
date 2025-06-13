#include "Motor.h"

Motoren::Motoren():status(true), staticSpeed(0) {
}

void Motoren::Beweeg() {
  SpeedL = staticSpeed;
  SpeedR = staticSpeed;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::turn(int l, int r) {
  SpeedL = l;
  SpeedR = r;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::rotateLeft90() {
  turn(-250, 250);
  int rotation = GetEncoderRight() + 350;
  while (rotation > GetEncoderRight()) {}
  Stop();
}

void Motoren::rotateRight90() {
  turn(250, -250);
  int rotation = GetEncoderLeft() + 350;
  while (rotation > GetEncoderLeft()) {}
  Stop();
}

void Motoren::Stop() {
  staticSpeed = 0;
  Motor.setSpeeds(0, 0);
}


void Motoren::SetSpeed(int speed) {
  SpeedL = speed;
  SpeedR = speed;
  staticSpeed = speed;
}

int Motoren::GetSpeed() {
  return staticSpeed;
}

int Motoren::GetEncoderRight() {
  return encodertje.getCountsRight();
}

int Motoren::GetEncoderLeft() {
  return encodertje.getCountsLeft();
}

int Motoren::getDistanceTraveled() {
  int leftCount = abs(Motoren::GetEncoderLeft());
  int rightCount = abs(Motoren::GetEncoderRight());
  int avgCount = (leftCount + rightCount) / 2;
  return avgCount / 75.81;
}