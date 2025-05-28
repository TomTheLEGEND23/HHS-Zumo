#include "Motor.h"

Motoren::Motoren():status(true),EBrake(false),CalibratieOffsetL(0),CalibratieOffsetR(0) {
}

void Motoren::SetEBrake(bool e) {
  EBrake = e;
}

void Motoren::Stop() {
  Motor.setSpeeds(0, 0);
}

void Motoren::GaVooruit(int speed) {
  SpeedL = speed;
  SpeedR = speed;
  if (!EBrake) {
    Motor.setSpeeds(SpeedL, SpeedR);
  }
}

void Motoren::GaAchteruit(int speed) {
  SpeedL = speed * -1;
  SpeedR = speed * -1;
  if (!EBrake) {
    Motor.setSpeeds(SpeedL, SpeedR);
  }
}

void Motoren::GaLinks(int speed) {
  if (SpeedL > 0) {
    SpeedL -= speed;
  }
  else if (SpeedR < 0) {
    SpeedL += speed;
  }
  else if (speed == 0) {
    SpeedL = speed * -1;
    SpeedR = speed;
  }

  if (!EBrake) {
    Motor.setSpeeds(SpeedL, SpeedR);
  }
}

void Motoren::GaRechts(int speed) {
  if (SpeedR > 0) {
    SpeedR -= speed;
  }
  else if (SpeedR < 0) {
    SpeedR += speed;
  }
  else if (SpeedR == 0) {
    SpeedR = speed  * -1;
    SpeedL = speed;
  }

  if (!EBrake) {
    Motor.setSpeeds(SpeedL, SpeedR);
  }
}