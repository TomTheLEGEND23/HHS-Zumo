#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>

class Xbee {
public:
    Xbee();
    void update();
    bool isButtonPressed(char c);
    char getLastChar();

private:
    bool available;
    char lastChar;
};

#endif
