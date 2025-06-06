#include "USBAPI.h"
#include "HardwareSerial.h"
#include "LineSensor.h"
#include <Arduino.h>

LineSensor::LineSensor() {
  zumoLineSensor.initFiveSensors();
}

void LineSensor::calibrateLineSensor(XBee &xbee) {
  Serial1.println("Put on white area");
  xbee.update();
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
  }
  Serial1.println("Calibrating...");
  for (uint16_t i = 0; i < 40; i++) {
    zumoLineSensor.calibrate();
  }

  Serial1.println("Put on black area");
  xbee.update();
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
  }
  Serial1.println("Calibrating...");
  zumoLineSensor.read(linesensorRawValue);
  for (uint16_t i = 0; i <= 4; i++) {
    blackThreshold[i] = linesensorRawValue[i] / 2 - 100;
    Serial1.println(linesensorRawValue[i]);
  }

  Serial1.println("Put on green area");
  xbee.update();
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
  }
  Serial1.println("Calibrating...");
  zumoLineSensor.read(linesensorRawValue);
  for (uint16_t i = 0; i <= 4; i++) {
    greenThreshold[i] = linesensorRawValue[i] - 30;
    Serial1.println(linesensorRawValue[i]);
  }
  for (uint16_t i = 0; i < 40; i++) {
    zumoLineSensor.calibrate();
  }

  Serial1.println("Finished line Calibration!");
}



String LineSensor::detectedLine(int l) {
  zumoLineSensor.read(lineSensorValues);
  zumoLineSensor.read(linesensorRawValue);
  if (linesensorRawValue[l] >= blackThreshold[l]) {
    return "Black";
  }
  else if (linesensorRawValue[l] >= greenThreshold[l]) {
    return "Green";
  } else {
    return "White";
  }
}

int LineSensor::readLine() {
  return zumoLineSensor.readLine(lineSensorValues);
}

unsigned int LineSensor::giveRawValue(int l) {
  zumoLineSensor.read(linesensorRawValue);
  return linesensorRawValue[l];
}