#ifndef PUSHBLOCK_H
#define PUSHBLOCK_H

#include <Arduino.h>

class PushBlock {
public:
  PushBlock();
  void pushBlock(int);
  bool completedPushing();

private:
  int distance;
  bool initial;
  bool completed;
  unsigned long windowStartTime;
  int toggleCount;
  bool lastReading;
  bool pushingObject;
  static const unsigned long toggleInterval = 1000;
  static const int toggleThreshold = 5;
};

#endif