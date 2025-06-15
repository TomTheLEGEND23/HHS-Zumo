#ifndef PUSHBLOCK_H
#define PUSHBLOCK_H

#include <Arduino.h>

class PushBlock {
public:
    PushBlock();
    void pushBlock(int);
    bool automationRunning;

private:
    bool pushBlockInitDone;
    bool notOverCircle;
};

#endif
