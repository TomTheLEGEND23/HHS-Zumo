#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include <Arduino.h>
#include "Motor.h"
#include "LineSensor.h"

class LineFollower {
public:
  LineFollower();
  void FollowLine(Motoren &motor, LineSensor &linesensor, const int, const int, const float);

private:
  bool onLeftCorner = false;
  bool onRightCorner = false;
  int previousLeftValue = 0;
  int previousRightValue = 0;
  bool leftRising = false;
  bool rightRising = false;
  bool LeftGray = false;
  bool RightGray = false;
  int rightSensor = 5;
  int leftSensor = 0;
  int leftPeak = 0;
  int rightPeak = 0;
};

#endif