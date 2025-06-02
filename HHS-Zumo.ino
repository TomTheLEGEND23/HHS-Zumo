#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU.h"
#include "XBee.h"

// Create objects for Zumo components
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Buzzer buzzer; // Added buzzer for victory sound
XBee xbee; // XBee communication module

// Line sensor array variables
#define NUM_SENSORS 5
uint16_t lineSensorValues[NUM_SENSORS];

// Proximity sensor variables
#define NUM_PROX_SENSORS 3
uint8_t proxSensorValues[NUM_PROX_SENSORS];

// PID control variables
float Kp = 0.4;   // Increased for better line following
float Ki = 0.002; 
float Kd = 3.0;   
int lastError = 0;
long integral = 0;

// Motor speed variables
const int MAX_SPEED = 200;
const int BASE_SPEED = 150;
const int SLOW_SPEED = 75; // Half speed for green lines

// State variables
enum RobotState {
  FOLLOWING_LINE,
  PAUSED_FOR_PITCH,
  SEARCHING_END_BLOCK,
  PUSHING_BLOCK,
  FINISHED
};

RobotState currentState = FOLLOWING_LINE;
unsigned long searchStartTime = 0;
bool victoryPlayed = false;
bool greenLineActive = false; // Track if robot is currently on green line

void setup() {
  Serial.begin(9600);
  
  // Initialize XBee communication
  xbee.initializeSerial();
  
  // Initialize proximity sensors
  proxSensors.initThreeSensors();
  
  // Initialize line sensors
  lineSensors.initFiveSensors();
  
  // Calibrate line sensors
  calibrateLineSensors();
  
  // Wait for button press to start
  Serial.println("Press A to start");
  Serial1.println("Press A to start");
  buttonA.waitForButton();
  Serial.println("Starting...");
  Serial1.println("Starting...");
  
  // Start sound
  buzzer.play("c32");
}

void loop() {
  // Check for XBee commands
  if (Serial1.available()) {
    char receivedChar = Serial1.read();
    xbee.receiveCharacter(receivedChar);
  }
  
  // Only run autonomous program if XBee program is running
  if (xbee.isLineFollowingProgramRunning()) {
    switch (currentState) {
      case FOLLOWING_LINE:
        followLine();
        checkForSpecialLines();
        break;
        
      case SEARCHING_END_BLOCK:
        searchForEndBlock();
        break;
        
      case PUSHING_BLOCK:
        pushBlock();
        break;
        
      case FINISHED:
        if (!victoryPlayed) {
          playVictorySound();
          victoryPlayed = true;
        }
        stopRobot();
        break;
    }
  } else {
    // Stop motors when program is not running (manual control mode)
    motors.setSpeeds(0, 0);
  }
  
  displayStatus();
  delay(10);
}

void calibrateLineSensors() {
  Serial.println("=== Line Sensor Calibration ===");
  Serial1.println("=== Line Sensor Calibration ===");
  
  // Standard calibration with movement
  Serial.println("Starting standard calibration (3 seconds)...");
  Serial1.println("Starting standard calibration (3 seconds)...");
  
  // Turn on LED for calibration
  ledYellow(1);
  
  // Calibrate for 3 seconds while robot moves
  for (uint16_t i = 0; i < 120; i++) {
    if (i > 30 && i <= 90) {
      motors.setSpeeds(100, -100);
    } else {
      motors.setSpeeds(-100, 100);
    }
    
    lineSensors.calibrate();
    delay(25);
  }
  
  motors.setSpeeds(0, 0);
  ledYellow(0);
  Serial.println("Standard calibration complete");
  Serial1.println("Standard calibration complete");
  
  // Color-specific calibration
  calibrateColors();
}

void calibrateColors() {
  Serial.println("=== Color Calibration ===");
  Serial1.println("=== Color Calibration ===");
  Serial.println("Place robot on BLACK line and press button A");
  Serial1.println("Place robot on BLACK line and press button A");
  
  buttonA.waitForButton();
  delay(500); // Debounce delay
  
  // Calibrate black line
  ledRed(1);
  Serial.println("Calibrating BLACK line...");
  Serial1.println("Calibrating BLACK line...");
  
  for (int i = 0; i < 100; i++) {
    lineSensors.calibrate();
    delay(10);
  }
  
  ledRed(0);
  Serial.println("Black line calibration complete");
  Serial1.println("Black line calibration complete");
  
  // Green line calibration
  Serial.println("Place robot on GREEN line and press button A");
  Serial1.println("Place robot on GREEN line and press button A");
  
  buttonA.waitForButton();
  delay(500); // Debounce delay
  
  ledGreen(1);
  Serial.println("Calibrating GREEN line...");
  Serial1.println("Calibrating GREEN line...");
  
  for (int i = 0; i < 100; i++) {
    lineSensors.calibrate();
    delay(10);
  }
  
  ledGreen(0);
  Serial.println("Green line calibration complete");
  Serial1.println("Green line calibration complete");
  
  // Brown line calibration
  Serial.println("Place robot on BROWN line and press button A");
  Serial1.println("Place robot on BROWN line and press button A");
  
  buttonA.waitForButton();
  delay(500); // Debounce delay
  
  ledYellow(1);
  Serial.println("Calibrating BROWN line...");
  Serial1.println("Calibrating BROWN line...");
  
  for (int i = 0; i < 100; i++) {
    lineSensors.calibrate();
    delay(10);
  }
  
  ledYellow(0);
  Serial.println("Brown line calibration complete");
  Serial1.println("Brown line calibration complete");
  
  // White/background calibration
  Serial.println("Place robot on WHITE/BACKGROUND and press button A");
  Serial1.println("Place robot on WHITE/BACKGROUND and press button A");
  
  buttonA.waitForButton();
  delay(500); // Debounce delay
  
  ledYellow(1);
  ledRed(1);
  Serial.println("Calibrating WHITE/BACKGROUND...");
  Serial1.println("Calibrating WHITE/BACKGROUND...");
  
  for (int i = 0; i < 100; i++) {
    lineSensors.calibrate();
    delay(10);
  }
  
  ledYellow(0);
  ledRed(0);
  Serial.println("White/background calibration complete");
  Serial1.println("White/background calibration complete");
  
  // Final confirmation
  buzzer.play("!T120 L8 c e g >c");
  Serial.println("=== ALL CALIBRATIONS COMPLETE ===");
  Serial1.println("=== ALL CALIBRATIONS COMPLETE ===");
  
  // Display calibration values for debugging
  displayCalibrationValues();
}

void displayCalibrationValues() {
  Serial.println("=== Calibration Values ===");
  Serial1.println("=== Calibration Values ===");
  
  Serial.print("Min values: ");
  Serial1.print("Min values: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(lineSensors.calibratedMinimumOn[i]);
    Serial1.print(lineSensors.calibratedMinimumOn[i]);
    if (i < NUM_SENSORS - 1) {
      Serial.print(", ");
      Serial1.print(", ");
    }
  }
  Serial.println();
  Serial1.println();
  
  Serial.print("Max values: ");
  Serial1.print("Max values: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(lineSensors.calibratedMaximumOn[i]);
    Serial1.print(lineSensors.calibratedMaximumOn[i]);
    if (i < NUM_SENSORS - 1) {
      Serial.print(", ");
      Serial1.print(", ");
    }
  }
  Serial.println();
  Serial1.println();
  Serial.println("===========================");
  Serial1.println("===========================");
}

void followLine() {
  // Read line sensors
  lineSensors.read(lineSensorValues);
  
  // Check for green line first to adjust speed
  bool greenLineDetected = false;
  int greenCount = 0;
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (lineSensorValues[i] > 200 && lineSensorValues[i] < 600) {
      greenCount++;
    }
  }
  
  if (greenCount >= 3) {
    greenLineDetected = true;
  }
  
  // Get line position (-2000 to 2000, 0 is center)
  int position = lineSensors.readLine(lineSensorValues);
  
  // Calculate error
  int error = position - 2000; // 2000 is center position
  
  // PID calculation
  integral += error;
  int derivative = error - lastError;
  int correction = Kp * error + Ki * integral + Kd * derivative;
  
  // Limit integral windup
  if (integral > 10000) integral = 10000;
  if (integral < -10000) integral = -10000;
  
  lastError = error;
  
  // Adjust base speed based on line color
  int currentBaseSpeed = greenLineDetected ? SLOW_SPEED : BASE_SPEED;
  
  // Calculate motor speeds
  int leftSpeed = currentBaseSpeed + correction;
  int rightSpeed = currentBaseSpeed - correction;
  
  // Constrain speeds
  leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
  
  // Set motor speeds
  motors.setSpeeds(leftSpeed, rightSpeed);
}

void checkForSpecialLines() {
  // Check for green line (reduce speed when detected)
  bool greenLineDetected = false;
  int greenCount = 0;
  
  // Check for green line - moderate sensor readings indicate green
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (lineSensorValues[i] > 200 && lineSensorValues[i] < 600) {
      greenCount++;
    }
  }
  
  // If majority of sensors detect green-like readings
  if (greenCount >= 3) {
    greenLineDetected = true;
    if (!greenLineActive) {
      Serial.println("Green line detected - reducing speed");
      Serial1.println("Green line detected - reducing speed");
      greenLineActive = true;
    }
  } else {
    if (greenLineActive) {
      Serial.println("Green line ended - normal speed");
      Serial1.println("Green line ended - normal speed");
      greenLineActive = false;
    }
  }
  
  // Check for brown line (simplified - you'll need proper color detection)
  // For now, using a simple heuristic based on sensor readings
  bool allSensorsLow = true;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (lineSensorValues[i] > 300) { // Adjust threshold as needed
      allSensorsLow = false;
      break;
    }
  }
  
  // If we lose the line completely, might be brown line area
  if (allSensorsLow) {
    Serial.println("Brown line detected, advancing 20cm");
    Serial1.println("Brown line detected, advancing 20cm");
    
    // Move forward 20cm (approximate)
    motors.setSpeeds(BASE_SPEED, BASE_SPEED);
    delay(800); // Adjust based on your robot's speed
    motors.setSpeeds(0, 0);
    
    currentState = SEARCHING_END_BLOCK;
    searchStartTime = millis();
    
    Serial.println("Starting block search");
    Serial1.println("Starting block search");
  }
}

void searchForEndBlock() {
  // Read proximity sensors
  proxSensors.read();
  
  uint8_t leftValue = proxSensors.countsFrontWithLeftLeds();
  uint8_t centerValue = proxSensors.countsFrontWithRightLeds();
  uint8_t rightValue = proxSensors.countsRightWithRightLeds();
  
  // Check if block is detected
  if (leftValue > 5 || centerValue > 5 || rightValue > 5) {
    currentState = PUSHING_BLOCK;
    buzzer.play("g32"); // Block found sound
    return;
  }
  
  // Search pattern - rotate to find block
  static unsigned long lastSearchMove = 0;
  static int searchDirection = 1;
  
  if (millis() - lastSearchMove > 500) {
    searchDirection *= -1;
    lastSearchMove = millis();
  }
  
  motors.setSpeeds(100 * searchDirection, -100 * searchDirection);
  
  // Timeout after 30 seconds
  if (millis() - searchStartTime > 30000) {
    currentState = FINISHED;
  }
}

void pushBlock() {
  // Read proximity sensors
  proxSensors.read();
  
  uint8_t centerValue = proxSensors.countsFrontWithRightLeds();
  
  if (centerValue > 5) {
    // Block still in front, keep pushing
    motors.setSpeeds(MAX_SPEED, MAX_SPEED);
  } else {
    // Block pushed out, mission complete!
    currentState = FINISHED;
  }
}

void stopRobot() {
  motors.setSpeeds(0, 0);
  
  Serial.println("VICTORY! Mission complete!");
  Serial1.println("VICTORY! Mission complete!");
}

void playVictorySound() {
  // Play a victory fanfare
  buzzer.play("!T120 L8 c d e f g a b >c");
  delay(1000);
  buzzer.play("!T150 L4 c e g >c");
  delay(500);
  buzzer.play("!T200 L16 c c c L8 c");
}

void displayStatus() {
  // Send current state via XBee
  String status = "Status: ";
  switch (currentState) {
    case FOLLOWING_LINE:
      status += "Following line";
      if (greenLineActive) {
        status += " (GREEN)";
      }
      break;
    case SEARCHING_END_BLOCK:
      status += "Searching for block";
      break;
    case PUSHING_BLOCK:
      status += "Pushing block";
      break;
    case FINISHED:
      status += "Mission complete!";
      break;
  }
  
  // Add program running status
  if (!xbee.isLineFollowingProgramRunning()) {
    status += " [MANUAL MODE]";
  }
  
  static unsigned long lastStatusUpdate = 0;
  // Send status every 2 seconds to avoid flooding
  if (millis() - lastStatusUpdate > 2000) {
    Serial.println(status);
    Serial1.println(status);
    lastStatusUpdate = millis();
  }
}