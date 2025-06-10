#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"

IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
Xbee xbee;
XbeeInputProcessing xbeeinputprocessing;
PrintInfo printinfo;

bool automationRunning = false;
bool lineSensorCalibration = false;
char ReChar;

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
  };
  // if (lineSensorCalibration) {
  //   imu.init();
  //   linesensor.calibrateLineSensor(xbee, motor);
  //   lineSensorCalibration = false;
  // };
  // if (Serial1.available()) {
  //   ReChar = xbee.readS1();
  //   xbeeinputprocessing.processKeyInput(ReChar, xbee, motor);
  // };

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
    int linePos = linesensor.detectedLine();

    if (linePos == -1) {
      // No line detected — move forward at half speed
      motor.SetSpeed(BASE_SPEED/2);
      motor.Beweeg();
      return;
    };

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
    // Serial1.println(linesensor.detectedLine());
  }
}