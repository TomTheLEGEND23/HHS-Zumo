#include "Xbee.h"
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

bool Xbee::isLastChar(char c)
{
    return lastChar == c; 
}

void Xbee::setLastChar(char c) {
    lastChar = c; 
}

char Xbee::readS1() {
    char receivedChar = Serial1.read();
    return receivedChar;
}