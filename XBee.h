#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>

class Xbee {
public:
    Xbee();
    void update();
    bool isButtonPressed(char c);

private:
    char lastChar;
    bool available;
};

#endif
