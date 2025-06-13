#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "PrintInfo.h"
#include <Zumo32U4.h>

IMU imu;
Xbee xbee;
Zumo32U4Encoders encoders;
Zumo32U4Motors motors;

//Variables:
#define BASE_SPEED 200
#define Balanve_SPEED 50
#define MAX_SPEED 400

// Encoder constants for distance calculation
#define COUNTS_PER_ROTATION 909.7  // CPR for 75:1 gear ratio
#define WHEEL_DIAMETER 32.0        // Wheel diameter in mm
#define COUNTS_PER_MM (COUNTS_PER_ROTATION / (PI * WHEEL_DIAMETER))

void driveDistance(float distanceMM, int speed = BASE_SPEED) {
  // Reset encoder counts
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  
  // Calculate target counts
  long targetCounts = abs(distanceMM * COUNTS_PER_MM);
  
  // Determine direction
  if (distanceMM > 0) {
    // Drive forward
    motors.setSpeeds(speed, speed);
  } else {
    // Drive backward
    motors.setSpeeds(-speed, -speed);
  }
  
  // Drive until target distance is reached
  while (true) {
    long leftCounts = abs(encoders.getCountsLeft());
    long rightCounts = abs(encoders.getCountsRight());
    
    // Use average of both encoders
    long averageCounts = (leftCounts + rightCounts) / 2;
    
    if (averageCounts >= targetCounts) {
      break;
    }
    
    delay(10); // Small delay to prevent overwhelming the processor
  }
  
  // Stop motors
  motors.setSpeeds(0, 0);
}

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
  }
    
  if (xbee.isButtonPressed('f')) {
    Serial1.println("Driving forward 50mm");
    driveDistance(50.0);
  }
  
  // Example: Drive backward 50mm when 'b' is pressed
  if (xbee.isButtonPressed('b')) {
    Serial1.println("Driving backward 50mm");
    driveDistance(-50.0);
  }
  
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
