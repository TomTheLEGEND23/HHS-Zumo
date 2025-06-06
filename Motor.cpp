#include "Motor.h"

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
