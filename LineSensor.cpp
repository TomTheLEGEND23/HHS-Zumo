#include "LineSensor.h"
#include <Arduino.h>

LineSensor::LineSensor() {
  zumoLineSensor.initFiveSensors();
}

void LineSensor::emitteroff() {
  zumoLineSensor.emittersOff();
}

void LineSensor::emitteron() {
  zumoLineSensor.emittersOn();
}

void LineSensor::calibrateLineSensor(Xbee &xbee, Motoren &motors) {
  Serial1.println("Starting calibration: spin robot slowly over the line.");

  // Wait for 'c' button press to start
  Serial1.println("Press 'c' to start calibration...");
  xbee.update();
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
  }

  // Spin robot slowly to sweep sensors over line and background
  motors.turn(200, -200);  // spin in place

  unsigned long startTime = millis();
  while (millis() - startTime < 1500) {  // calibrate for 2.5 seconds
    zumoLineSensor.calibrate();
  }
  motors.turn(-200, 200);  // spin in place opposite direction
  startTime = millis();
  while (millis() - startTime < 1500) {  // calibrate for 2.5 seconds
    zumoLineSensor.calibrate();
  }
  motors.Stop();

  Serial1.println("Calibration complete!");

  // Find and store minimum detection values
  unsigned int sensorValues[5];

  Serial1.println("Capturing minimum sensor values...");
  for (int i = 0; i < 100; i++) { // take 100 readings to find minimums
    zumoLineSensor.read(sensorValues); // read current sensor values
    for (int j = 0; j < 5; j++) {
      if (sensorValues[j] < MinimumDetection[j]) {
        MinimumDetection[j] = sensorValues[j];
      }
    }
    delay(10); // small delay to stabilize readings
  }

  Serial1.println("Minimum values per sensor:");
  for (int i = 0; i < 5; i++) {
    Serial1.print("Sensor ");
    Serial1.print(i);
    Serial1.print(": ");
    Serial1.println(MinimumDetection[i]);
  }
}


int LineSensor::giveRawValue(int l) {
  zumoLineSensor.read(linesensorRawValue);
  return linesensorRawValue[l];
}

int LineSensor::giveCalValue(int l) {
  int returning = giveRawValue(l) - MinimumDetection[l];
  return returning;
}

int LineSensor::detectedLine() {
  zumoLineSensor.readCalibrated(linesensorRawValue);

  unsigned long weightedSum = 0;
  unsigned int total = 0;

  // Map sensor indices to position values (0 to 4000)
  const int positionMap[5] = { 0, 1000, 2000, 3000, 4000 };

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