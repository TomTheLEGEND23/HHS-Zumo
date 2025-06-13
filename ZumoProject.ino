#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proxsensor;
Xbee xbee;
PrintInfo printinfo;

bool automationRunning = false;
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
    automationRunning = true;
  }
  // Stop line following
  if (xbee.isButtonPressed('o'))
  {
    Serial1.println("Program stopped");
    automationRunning = false;
    motor.Stop();
  }
  if (xbee.isButtonPressed('w') && !automationRunning)
  {
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('s') && !automationRunning)
  {
    motor.SetSpeed(-BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('a') && !automationRunning)
  {
    motor.turn(-BASE_SPEED, BASE_SPEED);
  }
  if (xbee.isButtonPressed('d') && !automationRunning)
  {
    motor.turn(BASE_SPEED, -BASE_SPEED);
  }
  if (xbee.isButtonPressed(' ') || xbee.isButtonPressed('0'))
  {
    // Serial1.println("Program stopped");
    motor.Stop();
    // automationRunning = false;
  }
  if (xbee.isButtonPressed('h'))
  {
    printinfo.printHelp();
  }
  if (xbee.isButtonPressed('x'))
  {
    printinfo.printDiagnostic();
  }

  // Push Block logic
  static bool pushBlockInitDone = false;
  static bool notOverCircle = true;
  if (automationRunning)
  {
    if (!pushBlockInitDone) {
      int distanceToTravel = motor.getDistanceTraveled() + 20;
      motor.SetSpeed(BASE_SPEED);
      motor.Beweeg();
      while (distanceToTravel > motor.getDistanceTraveled())
      {
      };
      motor.Stop();
      pushBlockInitDone = true;
    }
    motor.rotateLeft90();
    motor.rotateLeft90();
    automationRunning = false;
    while (((proxsensor.countsFrontLeft() <= 4) || (proxsensor.countsFrontRight() <= 4))) {
      Serial1.println("Turning");
      motor.turn(350, -350);
      delay(50);
      motor.Stop();
    };
    Serial1.println("Object found");
    proxsensor.printReadings();
    while (((proxsensor.countsFrontLeft() >= 3) || (proxsensor.countsFrontRight() >= 3)))
    {
      Serial1.println("Turning");
      motor.turn(350, -350);
      delay(30);
      motor.Stop();
      if (xbee.isButtonPressed('o'))
      {
        Serial1.println("Program stopped");
        motor.Stop();
        automationRunning = false;
      }
      
      while (((proxsensor.countsFrontLeft() >= 3) && (proxsensor.countsFrontRight() >= 3)))
      {
        Serial1.println("Object locked on");
        Serial1.print("Object Distance");
        proxsensor.printReadings();
        motor.SetSpeed(BASE_SPEED);
        motor.Beweeg();
        while (notOverCircle)
        {
          if (!(linesensor.detectedLine() == -1)) {
            notOverCircle = false;
            automationRunning = false;
            Serial1.println("line detected");
            motor.Stop();
          }
          // while (!((linesensor.giveCalValue(2) >= 700) || (linesensor.giveCalValue(0) >= 700) || (linesensor.giveCalValue(4) >= 700) )) {};
          // Serial1.println("line detected");
          // motor.Stop();
          // notOverCircle = false;
          // automationRunning = false;
        }
        if (xbee.isButtonPressed('o'))
        {
          Serial1.println("Program stopped");
          motor.Stop();
          automationRunning = false;
        }
      };
    }
  }
  else
  {
    pushBlockInitDone = false;
  }

  if (xbee.isButtonPressed('n'))
  {
    proxsensor.printReadings();
  };
}