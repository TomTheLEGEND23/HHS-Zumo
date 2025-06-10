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
bool goLeft = false;
bool goRight = false;

// Control parameters
#define BASE_SPEED 180  // Adjust depending on your robot's motor power
#define MAX_SPEED 280
#define KP 0.7  // Proportional gain

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
    if (imu.pitch() < -0.15) {
      motor.SetSpeed(300);
      motor.Beweeg();
      return;
    }

    int linePos = linesensor.detectedLine();

    if (linePos == -1) {
      // No line detected — move forward at half speed
      motor.SetSpeed(BASE_SPEED / 2);
      motor.Beweeg();
      return;
    }

    // Calculate error from center (2000)
    int S4 = linesensor.giveCalValue(4);
    int S2 = linesensor.giveCalValue(2);
    int S0 = linesensor.giveCalValue(0);

    int error = linePos - 2000;

    static int lastS4 = 0;
    static int lastS0 = 0;
    if (S4 > 250 && S4 < 500 && abs(S4 - lastS4) < 50) {
      goRight = true;
      Serial1.println("GrayDetected on Right!");
      error -= 500;
    }
    lastS4 = S4;

    if (S0 > 250 && S0 < 500 && abs(S0 - lastS0) < 50) {
      goLeft = true;
      Serial1.println("GrayDetected on Left!");
      error += 500;
    }
    lastS0 = S0;

    if (goLeft && S0 > 800) {
      int encoding = motor.GetEncoderRight() + 420;
      motor.turn(-200, 200);
      while (motor.GetEncoderRight() < encoding) {}
      motor.Stop();
      goLeft = false;
    }

    if (goRight && S4 > 800) {
      int encoding = motor.GetEncoderLeft() + 380;
      motor.turn(200, -200);
      while (motor.GetEncoderLeft() < encoding) {}
      motor.Stop();
      goRight = false;
    }
    
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
  }
  Serial1.println(imu.pitch());
}