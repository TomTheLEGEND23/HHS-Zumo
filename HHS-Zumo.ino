#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"

IMU imu;
Xbee xbee;

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
    Serial1.println("------------");
    
    delay(500); // Delay for readability
}
