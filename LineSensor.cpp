#include "LineSensor.h"
#include <Arduino.h>

LineSensor::LineSensor() {
  zumoLineSensor.initFiveSensors();
}

void LineSensor::calibrateLineSensor(Xbee &xbee, Motoren &motors) {
  Serial1.println("Starting calibration: spin robot slowly over the line.");


  // Wait for 'c' button press to start
//   Serial1.println("Press 'c' to start calibration...");
//   xbee.update();
//   while (!xbee.isButtonPressed('c')) {
//     xbee.update();
//   };

  // Alternative to the previous waiting for C input
  Serial1.println("Waiting for 2.5 seconds");
  delay(2500);

  // Spin robot slowly to sweep sensors over line and background
  motors.turn(200, -200);  // spin in place

  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {  // calibrate for 5 seconds
    zumoLineSensor.calibrate();
  };
  motors.turn(-200, 200);  // spin in place
  startTime = millis();
  while (millis() - startTime < 2000) {  // calibrate for 5 seconds
    zumoLineSensor.calibrate();
  };
  motors.Stop();

  Serial1.println("Calibration complete!");
}

unsigned int LineSensor::giveRawValue(int l) {
  zumoLineSensor.readCalibrated(linesensorRawValue);
  return linesensorRawValue[l];
}

int LineSensor::detectedLine() {
  zumoLineSensor.readCalibrated(linesensorRawValue);

  unsigned long weightedSum = 0;
  unsigned int total = 0;

  // Map sensor indices to position values (0 to 4000)
  const int positionMap[5] = {0, 1000, 2000, 3000, 4000};

  for (int i = 0; i < 5; i++) {
    int value = linesensorRawValue[i];  // 0 (white) to 1000 (black)
    weightedSum += (unsigned long)value * positionMap[i];
    total += value;
  }

  // If all sensors see white (very low readings), no line is detected
  if (total < 200) {  // Adjust threshold as needed based on surface
    return -1;
  }

  // Compute average position
  return weightedSum / total;  // Returns a value between 0 and 4000
}