#include "PushBlock.h"
#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"

extern IMU imu;
extern Motoren motor;
extern LineSensor linesensor;
extern ProximitySensors proxsensor;
extern Xbee xbee;
extern PrintInfo printinfo;

PushBlock::PushBlock()
{
    automationRunning = false;
}

void PushBlock::pushBlock(int BASE_SPEED)
{
    automationRunning = true;
    pushBlockInitDone = false;
    if (automationRunning)
    {
        if (!pushBlockInitDone)
        {
            int distanceToTravel = motor.getDistanceTraveled() + 20;
            motor.SetSpeed(BASE_SPEED);
            motor.Beweeg();
            while (distanceToTravel > motor.getDistanceTraveled())
            {
            };
            motor.Stop();
            motor.rotateLeft90();
            motor.rotateLeft90();
            pushBlockInitDone = true;
        }
        while (!((linesensor.giveCalValue(0) >= 700) || (linesensor.giveCalValue(1) >= 700) || (linesensor.giveCalValue(2) >= 700) || (linesensor.giveCalValue(3) >= 700) || (linesensor.giveCalValue(4) >= 700)))
        {
            while (((proxsensor.countsFrontLeft() <= 4) || (proxsensor.countsFrontRight() <= 4)))
            {
                Serial1.println("Searching");
                motor.turn(350, -350);
                delay(50);
                motor.Stop();
            };
            Serial1.println("Object found");
            proxsensor.printReadings();

            if (proxsensor.countsFrontLeft() <= proxsensor.countsFrontRight())
            {
                motor.turn(350, -350);
                delay(50);
                motor.Stop();
            }
            if (proxsensor.countsFrontLeft() >= proxsensor.countsFrontRight())
            {
                motor.turn(-350, 350);
                delay(50);
                motor.Stop();
            }
            if (abs(proxsensor.countsFrontLeft() - proxsensor.countsFrontRight()) <= 1)
            {
                motor.SetSpeed(BASE_SPEED);
                motor.Beweeg();
            }
        };
        Serial1.println("line detected");
        motor.Stop();
        notOverCircle = false;
        automationRunning = false;
    }
    else
    {
        pushBlockInitDone = false;
    }
}
