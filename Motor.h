#ifndef MOTOR_H
#define MOTOR_H

#include <Zumo32U4.h>

class Motoren {
public:
  Motoren();
  void Beweeg();
  void turn(int, int);
  void Stop();
  void SetSpeed(int);
  int GetSpeed();
  int GetEncoderLeft();
  int GetEncoderRight();
  int getDistanceTraveled();

private:
  Zumo32U4Motors Motor;
  Zumo32U4Encoders encodertje;
  bool status;
  int staticSpeed;
  int SpeedL;
  int SpeedR;
};

#endif