#include "XBee.h"
#include <ctype.h>

Xbee::Xbee() : lastChar(), available(false) {
    Serial1.begin(19200);
}

void Xbee::update() {
    if (Serial1.available()) {
        lastChar = Serial1.read();
        available = true;
    } else {
        available = false;
    }
}

bool Xbee::isButtonPressed(char c) {
    return available && tolower(lastChar) == tolower(c);
}

char Xbee::getLastChar() {
    return lastChar;
}