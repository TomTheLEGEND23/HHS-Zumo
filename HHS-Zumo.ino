#include <Wire.h>
#include <Zumo32U4.h>
#include "IMU.h"

// Create objects for Zumo components
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Buzzer buzzer; // Added buzzer for victory sound

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

void setup() {
  Serial.begin(9600);
  
  // Initialize proximity sensors
  proxSensors.initThreeSensors();
  
  // Initialize line sensors
  lineSensors.initFiveSensors();
  
  // Calibrate line sensors
  calibrateLineSensors();
  
  // Wait for button press to start
  lcd.clear();
  lcd.print("Press A");
  buttonA.waitForButton();
  lcd.clear();
  
  // Start sound
  buzzer.play("c32");
}

void loop() {
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
  
  displayStatus();
  delay(10);
}

void calibrateLineSensors() {
  lcd.clear();
  lcd.print("Calibrate");
  
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
  lcd.clear();
}

void followLine() {
  // Read line sensors
  lineSensors.read(lineSensorValues);
  
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
  
  // Calculate motor speeds
  int leftSpeed = BASE_SPEED + correction;
  int rightSpeed = BASE_SPEED - correction;
  
  // Constrain speeds
  leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
  
  // Set motor speeds
  motors.setSpeeds(leftSpeed, rightSpeed);
}

void checkForSpecialLines() {
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
    // Move forward 20cm (approximate)
    motors.setSpeeds(BASE_SPEED, BASE_SPEED);
    delay(800); // Adjust based on your robot's speed
    motors.setSpeeds(0, 0);
    
    currentState = SEARCHING_END_BLOCK;
    searchStartTime = millis();
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
  
  lcd.clear();
  lcd.print("VICTORY!");
  lcd.gotoXY(0, 1);
  lcd.print("SUCCESS");
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
  // Show current state on bottom line
  lcd.gotoXY(0, 1);
  switch (currentState) {
    case FOLLOWING_LINE:
      lcd.print("Following  ");
      break;
    case SEARCHING_END_BLOCK:
      lcd.print("Searching  ");
      break;
    case PUSHING_BLOCK:
      lcd.print("Pushing    ");
      break;
    case FINISHED:
      lcd.print("VICTORY!   ");
      break;
  }
}