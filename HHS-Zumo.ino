#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"

IMU imu;
Xbee xbee;

//Varibles:
#define BASE_SPEED 200
#define Balanve_SPEED 50
#define MAX_SPEED 400

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(19200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
}



void loop() {
 xbee.update();
 
  // Start calibration
  if (xbee.isButtonPressed('c')) {
    Serial1.println("Calibrating IMU...");
    imu.init();
    delay(1000);
  };
    
    Serial1.print("Pitch: ");
    Serial1.println(imu.pitch());
    
    // Check orientation based on pitch
    if (imu.pitch() > 0.03) {
      Serial1.println("Facing DOWN");
    } else if (imu.pitch() < -0.03) {
      Serial1.println("Facing UP");
    } else {
      Serial1.println("Sitting STRAIGHT");
    }
    Serial1.println("------------");
    
    delay(500); // Delay for readability
}
