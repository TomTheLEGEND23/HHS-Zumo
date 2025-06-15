#include "LineSensor.h"
#include <Arduino.h>

LineSensor::LineSensor() {
  zumoLineSensor.initFiveSensors();
}

void LineSensor::calibrateLineSensor(Xbee &xbee, Motoren &motors) {
  Serial1.println("Starting calibration.");

  // Wait for 'c' button press to start
  Serial1.println("Press 'c' to start calibration...");
  xbee.update();
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
  }

  // Find and store minimum detection values
  unsigned int sensorValues[5];

  Serial1.println("Capturing minimum sensor values...");
  for (int i = 0; i < 100; i++) {       // take 100 readings to find minimums
    zumoLineSensor.read(sensorValues);  // read current sensor values
    for (int j = 0; j < 5; j++) {
      if (sensorValues[j] < MinimumDetection[j]) {
        MinimumDetection[j] = sensorValues[j];
      }
    }
    delay(10);  // small delay to stabilize readings
  }

  Serial1.println("Minimum values per sensor:");
  for (int i = 0; i < 5; i++) {
    Serial1.print("Sensor ");
    Serial1.print(i);
    Serial1.print(": ");
    Serial1.println(MinimumDetection[i]);
  }
}

void LineSensor::updateSensors(int calValues[5]) {
  zumoLineSensor.read(linesensorRawValue);
  int val = linesensorRawValue[0] - MinimumDetection[0];
  calValues[0] = val;
  val = linesensorRawValue[1] - MinimumDetection[1];
  calValues[1] = val;
  val = linesensorRawValue[2] - MinimumDetection[2];
  calValues[2] = val;
  val = linesensorRawValue[3] - MinimumDetection[3];
  calValues[3] = val;
  val = linesensorRawValue[4] - MinimumDetection[4];
  calValues[4] = val;
}

int LineSensor::giveRawValue(int l) {
  return linesensorRawValue[l];
}

int LineSensor::giveCalValue(int l) {
  int returning = giveRawValue(l) - MinimumDetection[l];
  return returning;
}

int LineSensor::detectedLine(int l, int r) {

  int adjustedValues[5];
  int total = 0;
  long weightedSum = 0;

  const int positionMap[5] = { 0, 1000, 2000, 3000, 4000 };

  for (int i = l; i < r; i++) {
    adjustedValues[i] = linesensorRawValue[i] - MinimumDetection[i];
    if (adjustedValues[i] < 0) adjustedValues[i] = 0;

    total += adjustedValues[i];
    weightedSum += (long)adjustedValues[i] * positionMap[i];
  }

  if (total < 200) {  // total adjusted signal too low
    return -1;
  }

  return weightedSum / total;
}
