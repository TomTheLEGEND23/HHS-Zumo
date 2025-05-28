#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>
#include "WString.h"


class Xbee {
public:
    Xbee(char);
    bool isButtonPressed();
    
private:
    bool status;
    char storedButton;
    char pressedButton;
    bool pressed;
};

#endif  // XBEE_H