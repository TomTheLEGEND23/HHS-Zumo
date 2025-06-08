#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
Xbee xbee;

bool automationRunning = false;

// Control parameters
#define BASE_SPEED 400     // Adjust depending on your robot's motor power
#define MAX_SPEED 400
#define KP 1.0            // Proportional gain

void setup() {
}

void loop() {
  xbee.update();

  // Start calibration
  if (xbee.isButtonPressed('c')) {
    imu.init();
    linesensor.calibrateLineSensor(xbee, motor);
  }

  // Start line following
  if (xbee.isButtonPressed('p')) {
    automationRunning = true;
  }

  // Stop line following
  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    motor.Stop();
  }

  // Line following logic
  if (automationRunning) {
    int linePos = linesensor.detectedLine();

    if (linePos == -1) {
      // No line detected — move forward at half speed
      motor.SetSpeed(BASE_SPEED/2);
      motor.Beweeg();
      return;
    }

    // Calculate error from center (2000)
    int error = linePos - 2000;

    // Proportional correction
    int correction = KP * error;

    // Compute motor speeds
    int leftSpeed = BASE_SPEED + correction;
    int rightSpeed = BASE_SPEED - correction;

    // Constrain speeds
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

    // Drive using your Motoren class
    motor.turn(leftSpeed, rightSpeed);
    Serial1.println(linesensor.detectedLine());
  }
}