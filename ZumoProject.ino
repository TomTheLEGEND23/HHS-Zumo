#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"
#include "PushBlock.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proxsensor;
Xbee xbee;
PrintInfo printinfo;
PushBlock pushblock;

bool onLeftCorner = false;
bool onRightCorner = false;

// Control parameters
#define BASE_SPEED 400
#define MAX_SPEED 400

#define KP 0.5

void setup()
{
}

void loop()
{
  xbee.update();
  // Start calibration
  if (xbee.isButtonPressed('c'))
  {
    imu.init();
    linesensor.calibrateLineSensor(xbee, motor);
  };

  // Start line following
  if (xbee.isButtonPressed('p'))
  {
    Serial1.println("Program running");
    pushblock.pushBlock(BASE_SPEED);
  }
  // Stop line following
  if (xbee.isButtonPressed('o'))
  {
    Serial1.println("Program stopped");
    pushblock.automationRunning = false;
    motor.Stop();
  }
  if (xbee.isButtonPressed('w') && !pushblock.automationRunning)
  {
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('s') && !pushblock.automationRunning)
  {
    motor.SetSpeed(-BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('a') && !pushblock.automationRunning)
  {
    motor.turn(-BASE_SPEED, BASE_SPEED);
  }
  if (xbee.isButtonPressed('d') && !pushblock.automationRunning)
  {
    motor.turn(BASE_SPEED, -BASE_SPEED);
  }
  if (xbee.isButtonPressed(' ') || xbee.isButtonPressed('0'))
  {
    // Serial1.println("Program stopped");
    motor.Stop();
    pushblock.automationRunning = false;
  }
  if (xbee.isButtonPressed('h'))
  {
    printinfo.printHelp();
  }
  if (xbee.isButtonPressed('x'))
  {
    printinfo.printDiagnostic();
  }

  if (xbee.isButtonPressed('n'))
  {
    proxsensor.printReadings();
  };
}