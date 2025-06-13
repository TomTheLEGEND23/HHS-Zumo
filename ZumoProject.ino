#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proxsensor;
Xbee xbee;
PrintInfo printinfo;

bool automationRunning = false;

// Control parameters
#define BASE_SPEED 400     // Adjust depending on your robot's motor power
#define MAX_SPEED 400
#define KP 0.9            // Proportional gain

void setup() {
}

void loop() {
  xbee.update();
  // Start calibration
  if (xbee.isButtonPressed('c')) {
    imu.init();
    linesensor.calibrateLineSensor(xbee, motor);
  };

  // Start line following
  if (xbee.isButtonPressed('p')) {
    Serial1.println("Program running");
    automationRunning = true;
  }
  // Stop line following
  if (xbee.isButtonPressed('o')) {
    Serial1.println("Program stopped");
    automationRunning = false;
    motor.Stop();
  }
  if (xbee.isButtonPressed('w') && !automationRunning) {
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('s') && !automationRunning) {
    motor.SetSpeed(-BASE_SPEED);
    motor.Beweeg();
  }
  if (xbee.isButtonPressed('a') && !automationRunning) {
    motor.turn(-BASE_SPEED, BASE_SPEED);
  }
  if (xbee.isButtonPressed('d') && !automationRunning) {
    motor.turn(BASE_SPEED, -BASE_SPEED);
  }
  if (xbee.isButtonPressed(' ') || xbee.isButtonPressed('0')) {
    Serial1.println("Program stopped");
    automationRunning = false;
    motor.Stop();
  }
  if (xbee.isButtonPressed('h')) {
    printinfo.printHelp();
  }
  if (xbee.isButtonPressed('x')) {
    printinfo.printDiagnostic();
  }

  // Line following logic
  if (automationRunning) {
    int distanceToTravel = motor.getDistanceTraveled() + 20;
    motor.SetSpeed(BASE_SPEED);
    motor.Beweeg();
    while (distanceToTravel > motor.getDistanceTraveled()) {};
    motor.Stop();
    while ((proxsensor.countsFrontLeft() <= 4) && (proxsensor.countsFrontRight() <= 4)) {
      Serial1.println("Turning");
      motor.turn(350, -350);
      delay(50);
      motor.Stop();
    };
    Serial1.println("Object found");
    if ((proxsensor.countsFrontLeft() >= 3) && (proxsensor.countsFrontRight() >= 3) && proxsensor.countsFrontLeft() == proxsensor.countsFrontRight()) {
      Serial1.println("Object locked on");
      motor.SetSpeed(BASE_SPEED/2);
      motor.Beweeg();
      if (xbee.isButtonPressed("o")) {
        motor.Stop();
      };
    }
  }
}