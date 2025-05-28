#include <Zumo32U4.h>
#include <Wire.h>

class CalibrateZumo {
public:
  CalibrateZumo();
  void Calibrate();
  int MagCalibratedX();
  int MagCalibratedY();
private:
  Zumo32U4IMU zumoimu;
  Zumo32U4Motors motors;
  int MagX;
  int MagY;
  bool status;
};