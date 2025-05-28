#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "CalibrateZumo.h"
#include "LineSensor.h"
#include "ProximitySensors.h"

IMU imu;
CalibrateZumo calibration;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
Xbee xbeeCalibrate('c');

void setup() {
}

void loop() {
  if (xbeeCalibrate.isButtonPressed()) {
    linesensor.calibrateLineSensor();
    calibration.Calibrate();
    imu.init(calibration.MagCalibratedX(), calibration.MagCalibratedY());
  }
}
