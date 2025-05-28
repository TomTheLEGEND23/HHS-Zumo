#include "CalibrateZumo.h"

CalibrateZumo::CalibrateZumo()
  : status(false){};

void CalibrateZumo::Calibrate() {
  Serial1.begin(19200);
  Serial.println("Starting magnetic and motor calibration");
  Serial1.println("Starting magnetic and motor calibration");
  // --- Magnetometer Calibration ---
  float magMinX = 10000, magMinY = 10000;
  float magMaxX = -10000, magMaxY = -10000;

  const int rotationTime = 500;
  const int pauseTime = 300;
  const int cycles = 20;

  int cycleCount = 0;
  unsigned long lastActionTime = 0;
  bool rotating = true;
  bool finished = false;

  zumoimu.enableDefault();
  motors.setSpeeds(0, 0);
  unsigned long currentTime = millis();

  while (!finished) {
    currentTime = millis();

    if (rotating && currentTime - lastActionTime >= rotationTime) {
      motors.setSpeeds(0, 0);
      rotating = false;
      lastActionTime = currentTime;

    } else if (!rotating && currentTime - lastActionTime >= pauseTime) {
      zumoimu.read();
      int16_t mx = zumoimu.m.x;
      int16_t my = zumoimu.m.y;

      if (mx < magMinX) magMinX = mx;
      if (mx > magMaxX) magMaxX = mx;
      if (my < magMinY) magMinY = my;
      if (my > magMaxY) magMaxY = my;

      cycleCount++;
      if (cycleCount >= cycles) {
        motors.setSpeeds(0, 0);
        finished = true;

        MagX = (magMaxX + magMinX) / 2.0;
        MagY = (magMaxY + magMinY) / 2.0;
      }
    } else if (!rotating && currentTime - lastActionTime < pauseTime) {
      // wait
    } else {
      motors.setSpeeds(200, -200);
      rotating = true;
      lastActionTime = currentTime;
    }
  }
}


int CalibrateZumo::MagCalibratedX() {
  return MagX;
};

int CalibrateZumo::MagCalibratedY() {
  return MagY;
};