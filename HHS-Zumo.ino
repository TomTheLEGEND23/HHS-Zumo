#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"
#include <Zumo32U4.h>

IMU imu;
Xbee xbee;
Motoren motor; // Use your Motor class instead of Zumo32U4 objects

//Variables:
#define BASE_SPEED 200
#define Balance_SPEED 50
#define MAX_SPEED 400

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(19200);
  Serial1.begin(19200);
}

void loop() {
  xbee.update();
  
  // Start calibration
  if (xbee.isButtonPressed('c')) {
    Serial1.println("Calibrating IMU...");
    imu.init();
    delay(1000);
  }
    
  if (xbee.isButtonPressed('f')) {
    Serial1.println("Driving forward 50mm");
    motor.driveDistance(50.0);
  }
  
  // Example: Drive backward 50mm when 'b' is pressed
  if (xbee.isButtonPressed('b')) {
    Serial1.println("Driving backward 50mm");
    motor.driveDistance(-50.0);
  }
  
  Serial1.print("Pitch: ");
  Serial1.println(imu.pitch());
  
  // Check orientation based on pitch
  if (imu.pitch() > 0.03) {
    Serial1.println("Facing DOWN");
    motor.driveDistance(-2.0, Balance_SPEED);
  } else if (imu.pitch() < -0.03) {
    Serial1.println("Facing UP");
    motor.driveDistance(2.0, Balance_SPEED);
  } else {
    Serial1.println("Sitting STRAIGHT");
  }
  Serial1.println("------------");
  
  delay(500); // Delay for readability
}
