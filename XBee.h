#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>

class Xbee {
public:
    Xbee();
    void update();
    bool isButtonPressed(char c);
    char getLastChar();
    bool isLastChar(char c);
    void setLastChar(char c);
    char readS1();

private:
    bool available;
    char lastChar;
};

#endif
