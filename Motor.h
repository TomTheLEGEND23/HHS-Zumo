#ifndef MOTOR_H
#define MOTOR_H

#include <Zumo32U4.h>

class Motoren {
public:
  Motoren();
  void GaVooruit(int);
  void GaLinks(int);
  void GaRechts(int);
  void GaAchteruit(int);
  void Stop();
  void SetStaticSpeed(int);
  
  // New methods for encoder functionality
  void resetEncoders();
  float getDistanceTraveled();
  long getLeftEncoderCount();
  long getRightEncoderCount();

private:
  Zumo32U4Motors Motor;
  bool status;
  int staticSpeed;
  int SpeedL;
  int SpeedR;
  bool EBrake;
};

#endif