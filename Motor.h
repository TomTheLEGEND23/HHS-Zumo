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
  void SetEBrake(bool);
  void SetCalibration(int, int);

private:
  Zumo32U4Motors Motor;
  int CalibratieOffsetL;
  int CalibratieOffsetR;
  bool status;
  int SpeedL;
  int SpeedR;
  bool EBrake;
};

#endif  // XBEE_H