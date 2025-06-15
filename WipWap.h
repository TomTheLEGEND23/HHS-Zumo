#ifndef WIPWAP_H
#define WIPWAP_H

#include <Arduino.h>

class WipWap {
public:
  WipWap();
  void Wippen();
  bool completedWippen();

private:
  bool completed;
  int distance;
};

#endif