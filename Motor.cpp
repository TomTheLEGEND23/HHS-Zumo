#include "Motor.h"

Motoren::Motoren()
  : status(true), staticSpeed(0) {
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
  int rotation = GetEncoderRight() + 360;
  while (rotation > GetEncoderRight()) {}
  Stop();
}

void Motoren::rotateRight90() {
  turn(250, -250);
  int rotation = GetEncoderLeft() + 360;
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

void Motoren::ResetEncoder() {
  encodertje.getCountsAndResetLeft();
  encodertje.getCountsAndResetRight();
}