#include <Arduino.h>
#include "WCharacter.h"
#include "WString.h"
#include "Xbee.h"


Xbee::Xbee(char c):status(true), pressed(false), storedButton(c) {
    Serial1.begin(19200);
}

bool Xbee::isButtonPressed() {
    pressedButton = Serial1.read();  // get one character
    if (tolower(pressedButton) == storedButton) {
        return true;
  }
  return false;
}