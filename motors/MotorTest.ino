

#include <Wire.h>
#include <Zumo32U4.h>
#include "Motor_HHS.h"

Motor_HHS motorLinks("links", 10, 16);
Motor_HHS motorRechts("rechts", 9, 15);

void setup() {
  motorLinks.init();
  motorRechts.init();
  motorLinks.setSnelheid(0);
  motorRechts.setSnelheid(0);
}

void loop() {
motorLinks.setSnelheid(200);
}