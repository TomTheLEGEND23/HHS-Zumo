#include "HardwareSerial.h"
#include "LineSensor.h"
#include <Arduino.h>

LineSensor::LineSensor() {
  zumoLineSensor.initFiveSensors();
}

void LineSensor::calibrateLineSensor() {
  Xbee xbeeContinue('c');
  Serial1.begin(19200);
  Serial.begin(9600);
  Serial1.println("Put on white area");
  Serial.println("Put on white area");
  while (!xbeeContinue.isButtonPressed()) {
    // wait here
  }
  Serial1.println("Calibrating...");
  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 30; i++) {
    zumoLineSensor.calibrate();
  }
  Serial1.println("Put on black area");
  Serial.println("Put on black area");
  while (!xbeeContinue.isButtonPressed()) {
    // wait here
  }
  Serial1.println("Calibrating...");
  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 30; i++) {
    zumoLineSensor.calibrate();
  }
  Serial1.println("Put on green area");
  Serial.println("Put on green area");
  while (!xbeeContinue.isButtonPressed()) {
    // wait here
  }
  Serial1.println("Calibrating...");
  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 30; i++) {
    zumoLineSensor.calibrate();
  }
  Serial1.println("Put on brown area");
  Serial.println("Put on brown area");
  while (!xbeeContinue.isButtonPressed()) {
    // wait here
  }
  Serial1.println("Calibrating...");
  Serial.println("Calibrating...");
  for (uint16_t i = 0; i < 30; i++) {
    zumoLineSensor.calibrate();
  }
  Serial1.println("Finished line Calibration, press c to continue");
  Serial.println("Finished line Calibration, press c to continue");
}

String LineSensor::detectedLine(int l) {
  zumoLineSensor.readCalibrated(lineSensorValues);
  if (lineSensorValues[l] >= blackThreshold) {
    return "Black";
  } else if (lineSensorValues[l] >= grayThresholdbottom && lineSensorValues[l] <= grayThresholdtop) {
    return "Gray";
  } else if (lineSensorValues[l] >= greenThresholdbottom && lineSensorValues[l] <= greenThresholdtop) {
    return "Green";
  } else if (lineSensorValues[l] >= brownThresholdbottom && lineSensorValues[l] <= brownThresholdtop) {
    return "Brown";
  } else {
    return "White";
  }
}

unsigned int LineSensor::giveRawValue(int l) {
  zumoLineSensor.readCalibrated(lineSensorValues);
  return lineSensorValues[l];
}