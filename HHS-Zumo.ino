// HHS-Zumo Robot Control System
// Enhanced with color calibration, line following, and cube-pushing functionality

#include "IMU.h"
#include "XBee.h"
#include "Motor.h"
#include "LineSensor.h"
#include "ProximitySensors.h"
#include "Buzzer.h"

// Global objects
IMU imu;
Motoren motor;
LineSensor linesensor;
ProximitySensors proximitysensor;
XBee xbee;
Buzzer buzzer;

// State variables
bool moving = false;
bool automationRunning = false;
bool calibrationMode = false;
bool cubePushingMode = false;
int motorspeed = 300;
int halfspeed = 150;

// Line following variables
int previousLinePosition = 2000;
unsigned long lastHighTime = 0;
bool AllWhite = false;

// Color calibration variables - using PROGMEM to save RAM
struct ColorCalibration {
  uint16_t black[5];
  uint16_t green[5];
  uint16_t brown[5];
  uint16_t white[5];
  bool calibrated;
} colorCal = {0};

// Cube pushing variables
enum CubePushState {
  DRIVE_TO_OBJECT,
  ROTATE_TO_FIND,
  PUSH_OBJECT,
  STOP_COMPLETE
};
CubePushState cubePushState = DRIVE_TO_OBJECT;

void setup() {
  Serial1.begin(19200);
  Serial1.println(F("HHS-Zumo Robot System Starting..."));
  
  // Initialize IMU
  if (imu.init()) {
    Serial1.println(F("IMU initialized successfully"));
  } else {
    Serial1.println(F("IMU initialization failed"));
  }
  
  // Initialize encoders
  motor.resetEncoders();
  
  Serial1.println(F("System ready!"));
  Serial1.println(F("Press 'q' for full command list"));
  Serial1.println(F("Quick: c=cal k=line p=cube o=stop"));
}

void loop() {
  xbee.update();

  // Handle button presses
  if (xbee.isButtonPressed('c')) {
    calibrationMode = true;
    automationRunning = false;
    cubePushingMode = false;
    motor.Stop();
    calibrateColors();
  }

  if (xbee.isButtonPressed('k')) {
    if (colorCal.calibrated) {
      automationRunning = true;
      cubePushingMode = false;
      calibrationMode = false;
      xbee.setLineFollowingProgram(true);
      Serial1.println(F("Line following started"));
    } else {
      Serial1.println(F("Please calibrate colors first!"));
    }
  }

  if (xbee.isButtonPressed('p')) {
    if (colorCal.calibrated) {
      cubePushingMode = true;
      automationRunning = false;
      calibrationMode = false;
      xbee.setLineFollowingProgram(false);
      cubePushState = DRIVE_TO_OBJECT;
      Serial1.println(F("Cube pushing mode started"));
    } else {
      Serial1.println(F("Please calibrate colors first!"));
    }
  }

  if (xbee.isButtonPressed('o')) {
    automationRunning = false;
    cubePushingMode = false;
    calibrationMode = false;
    motor.Stop();
    moving = false;
    xbee.setLineFollowingProgram(false);
    Serial1.println(F("All operations stopped"));
  }

  if (xbee.isButtonPressed('v')) {
    displayCalibrationValues();
  }

  if (xbee.isButtonPressed('t')) {
    testEncoders();
  }

  // Manual motor control commands (only when not in automatic modes)
  if (!automationRunning && !cubePushingMode && !calibrationMode) {
    if (xbee.isButtonPressed('w')) {
      motor.GaVooruit(200);
      Serial1.println(F("Manual: Moving forward"));
    }
    
    if (xbee.isButtonPressed('s')) {
      motor.GaAchteruit(200);
      Serial1.println(F("Manual: Moving backward"));
    }
    
    if (xbee.isButtonPressed('a')) {
      motor.GaLinks(250);
      Serial1.println(F("Manual: Turning left"));
    }
    
    if (xbee.isButtonPressed('d')) {
      motor.GaRechts(250);
      Serial1.println(F("Manual: Turning right"));
    }
    
    if (xbee.isButtonPressed('x')) {
      motor.Stop();
      Serial1.println(F("Manual: Stopped"));
    }
  }

  // Handle manual control attempts during autonomous modes
  if (automationRunning || cubePushingMode || calibrationMode) {
    if (xbee.isButtonPressed('w') || xbee.isButtonPressed('s') || 
        xbee.isButtonPressed('a') || xbee.isButtonPressed('d') || 
        xbee.isButtonPressed('x')) {
      Serial1.println(F("Manual control disabled during autonomous operation"));
      Serial1.println(F("Press 'o' to stop autonomous mode first"));
    }
  }

  // Display help/commands
  if (xbee.isButtonPressed('q')) {
    displayCommands();
  }

  // Buzzer sound controls (number keys 1-9)
  if (xbee.isButtonPressed('1')) {
    Serial1.println(F("Playing startup sound"));
    buzzer.playStartup();
  }
  
  if (xbee.isButtonPressed('2')) {
    Serial1.println(F("Playing victory fanfare"));
    buzzer.playVictory();
  }
  
  if (xbee.isButtonPressed('3')) {
    Serial1.println(F("Playing error sound"));
    buzzer.playError();
  }
  
  if (xbee.isButtonPressed('4')) {
    Serial1.println(F("Playing siren"));
    buzzer.playSiren();
  }
  
  if (xbee.isButtonPressed('5')) {
    Serial1.println(F("Playing Imperial March"));
    buzzer.playImperialMarch();
  }
  
  if (xbee.isButtonPressed('6')) {
    Serial1.println(F("Playing R2-D2 beeps"));
    buzzer.playR2D2();
  }
  
  if (xbee.isButtonPressed('7')) {
    Serial1.println(F("Playing R2-D2 scream"));
    buzzer.playR2D2Scream();
  }
  
  if (xbee.isButtonPressed('8')) {
    Serial1.println(F("Playing Tetris theme"));
    buzzer.playTetris();
  }
  
  if (xbee.isButtonPressed('9')) {
    Serial1.println(F("Playing Mario theme"));
    buzzer.playMarioTheme();
  }

  // Execute appropriate mode
  if (cubePushingMode) {
    executeCubePushing();
  } else if (automationRunning && xbee.isLineFollowingProgramRunning()) {
    followLine();
  }
}

void calibrateColors() {
  Serial1.println(F("=== COLOR CALIBRATION MODE ==="));
  Serial1.println(F("Place robot on BLACK surface and press 'c'"));
  
  // Wait for confirmation
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
    delay(50);
  }
  
  // Calibrate black
  Serial1.println(F("Calibrating BLACK..."));
  for (int i = 0; i < 5; i++) {
    colorCal.black[i] = linesensor.giveRawValue(i);
    Serial1.print(F("Sensor "));
    Serial1.print(i);
    Serial1.print(F(": "));
    Serial1.println(colorCal.black[i]);
  }
  
  Serial1.println(F("BLACK calibration complete! Place robot on GREEN surface and press 'c'"));
  
  // Wait for confirmation before proceeding to green
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
    delay(50);
  }
  
  // Calibrate green
  Serial1.println(F("Calibrating GREEN..."));
  for (int i = 0; i < 5; i++) {
    colorCal.green[i] = linesensor.giveRawValue(i);
    Serial1.print(F("Sensor "));
    Serial1.print(i);
    Serial1.print(F(": "));
    Serial1.println(colorCal.green[i]);
  }
  
  Serial1.println(F("GREEN calibration complete! Place robot on BROWN surface and press 'c'"));
  
  // Wait for confirmation before proceeding to brown
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
    delay(50);
  }
  
  // Calibrate brown
  Serial1.println(F("Calibrating BROWN..."));
  for (int i = 0; i < 5; i++) {
    colorCal.brown[i] = linesensor.giveRawValue(i);
    Serial1.print(F("Sensor "));
    Serial1.print(i);
    Serial1.print(F(": "));
    Serial1.println(colorCal.brown[i]);
  }
  
  Serial1.println(F("BROWN calibration complete! Place robot on WHITE surface and press 'c'"));
  
  // Wait for confirmation before proceeding to white
  while (!xbee.isButtonPressed('c')) {
    xbee.update();
    delay(50);
  }
  
  // Calibrate white
  Serial1.println(F("Calibrating WHITE..."));
  for (int i = 0; i < 5; i++) {
    colorCal.white[i] = linesensor.giveRawValue(i);
    Serial1.print(F("Sensor "));
    Serial1.print(i);
    Serial1.print(F(": "));
    Serial1.println(colorCal.white[i]);
  }
  
  colorCal.calibrated = true;
  Serial1.println(F("=== CALIBRATION COMPLETE ==="));
  calibrationMode = false;
}

void displayCalibrationValues() {
  if (!colorCal.calibrated) {
    Serial1.println(F("No calibration data available"));
    return;
  }
  
  Serial1.println(F("=== CALIBRATION VALUES ==="));
  Serial1.println(F("BLACK: "));
  for (int i = 0; i < 5; i++) {
    Serial1.print(colorCal.black[i]);
    if (i < 4) Serial1.print(", ");
  }
  Serial1.println();
  
  Serial1.println(F("GREEN: "));
  for (int i = 0; i < 5; i++) {
    Serial1.print(colorCal.green[i]);
    if (i < 4) Serial1.print(F(", "));
  }
  Serial1.println();
  
  Serial1.println(F("BROWN: "));
  for (int i = 0; i < 5; i++) {
    Serial1.print(colorCal.brown[i]);
    if (i < 4) Serial1.print(F(", "));
  }
  Serial1.println();
  
  Serial1.println(F("WHITE: "));
  for (int i = 0; i < 5; i++) {
    Serial1.print(colorCal.white[i]);
    if (i < 4) Serial1.print(F(", "));
  }
  Serial1.println();
}

// Color detection using constants instead of strings to save memory
#define COLOR_BLACK 0
#define COLOR_GREEN 1
#define COLOR_BROWN 2
#define COLOR_WHITE 3
#define COLOR_UNKNOWN 4

int detectColorInt(int sensorIndex) {
  if (!colorCal.calibrated) return COLOR_UNKNOWN;
  
  uint16_t currentValue = linesensor.giveRawValue(sensorIndex);
  
  // Calculate distances to each calibrated color
  uint16_t blackDist = abs(currentValue - colorCal.black[sensorIndex]);
  uint16_t greenDist = abs(currentValue - colorCal.green[sensorIndex]);
  uint16_t brownDist = abs(currentValue - colorCal.brown[sensorIndex]);
  uint16_t whiteDist = abs(currentValue - colorCal.white[sensorIndex]);
  
  // Find the closest match
  uint16_t minDist = blackDist;
  int closestColor = COLOR_BLACK;
  
  if (greenDist < minDist) {
    minDist = greenDist;
    closestColor = COLOR_GREEN;
  }
  if (brownDist < minDist) {
    minDist = brownDist;
    closestColor = COLOR_BROWN;
  }
  if (whiteDist < minDist) {
    minDist = whiteDist;
    closestColor = COLOR_WHITE;
  }
  
  return closestColor;
}

String detectColor(int sensorIndex) {
  int colorCode = detectColorInt(sensorIndex);
  switch (colorCode) {
    case COLOR_BLACK: return F("Black");
    case COLOR_GREEN: return F("Green");
    case COLOR_BROWN: return F("Brown");
    case COLOR_WHITE: return F("White");
    default: return F("Unknown");
  }
}

void followLine() {
  int linePosition = linesensor.readLine();  // 0 to 4000
  int centerColor = detectColorInt(2);
  bool centerSeesLine = (centerColor == COLOR_BLACK);
  static int aggressiveness = 80;  // Base steering power
  
  if (!moving) {
    motor.GaVooruit(motorspeed);
    moving = true;
  }

  // Check for special lines (green for speed reduction)
  int leftColor = detectColorInt(0);
  int rightColor = detectColorInt(4);
  
  if (leftColor == COLOR_GREEN || rightColor == COLOR_GREEN || centerColor == COLOR_GREEN) {
    motor.GaVooruit(halfspeed);
    Serial1.println(F("Green line detected - reducing speed"));
    delay(500);
    return;
  }

  // Adaptive Aggressiveness
  if (!centerSeesLine) {
    aggressiveness += 40;  // Increase correction force
    if (aggressiveness > 400) aggressiveness = 400;
  } else {
    aggressiveness = 80;  // Reset when center sees the line
  }

  // Check if all sensors see white (end of line or gap)
  if (linePosition == 4000 || linePosition == 0) {
    AllWhite = true;
    for (int i = 0; i < 5; i++) {
      if (detectColorInt(i) != COLOR_WHITE) {
        AllWhite = false;
        break;
      }
    }
  } else {
    AllWhite = false;
  }

  // Line following logic
  if (AllWhite) {
    motor.GaVooruit(motorspeed);
  } else if (linePosition > 2000) {
    int rightPower = map(linePosition, 2000, 4000, aggressiveness / 2, aggressiveness);
    motor.GaRechts(rightPower);
  } else if (linePosition < 2000) {
    int leftPower = map(linePosition, 0, 2000, aggressiveness, aggressiveness / 2);
    motor.GaLinks(leftPower);
  }
}

void executeCubePushing() {
  static unsigned long stateStartTime = 0;
  static float initialDistance = 0;
  static int rotationAttempts = 0;
  
  // Declare variables outside switch to avoid "jump to case label" errors
  int objectStrength;
  bool blackLineDetected;
  float pushDistance;
  int direction;
  
  switch (cubePushState) {
    case DRIVE_TO_OBJECT:
      // Drive forward 30cm using encoders
      if (stateStartTime == 0) {
        stateStartTime = millis();
        motor.resetEncoders();
        Serial1.println(F("Driving forward 30cm..."));
      }
      
      motor.GaVooruit(200);
      
      // Debug output every 2 seconds
      if (millis() - stateStartTime > 2000 && (millis() - stateStartTime) % 2000 < 100) {
        float currentDistance = motor.getDistanceTraveled();
        Serial1.print(F("Distance traveled: "));
        Serial1.print(currentDistance);
        Serial1.println(F(" cm"));
      }
      
      // Check if we've traveled 30cm
      if (motor.getDistanceTraveled() >= 30.0) {
        motor.Stop();
        cubePushState = ROTATE_TO_FIND;
        stateStartTime = 0;
        rotationAttempts = 0;
        Serial1.print(F("30cm reached (actual: "));
        Serial1.print(motor.getDistanceTraveled());
        Serial1.println(F(" cm), starting rotation to find object..."));
      }
      
      // Safety timeout - if driving for more than 15 seconds, something is wrong
      if (millis() - stateStartTime > 15000) {
        motor.Stop();
        cubePushState = STOP_COMPLETE;
        Serial1.println(F("Drive timeout - check encoders!"));
      }
      break;
      
    case ROTATE_TO_FIND:
      // Rotate in place to find object using proximity sensors
      if (stateStartTime == 0) {
        stateStartTime = millis();
        Serial1.println(F("Rotating to find object..."));
      }
      
      // Stop to take a reading
      motor.Stop();
      delay(100);  // Brief pause for stable reading
      
      objectStrength = proximitysensor.scanForObject();
      
      if (objectStrength > 8) {  // Strong enough detection threshold
        motor.Stop();
        cubePushState = PUSH_OBJECT;
        stateStartTime = 0;
        Serial1.print(F("Object found with strength: "));
        Serial1.println(objectStrength);
        Serial1.println(F("Starting push sequence..."));
      } else {
        // Rotate in small increments to scan for object
        motor.GaRechts(120);
        delay(200);  // Rotate for 200ms
        motor.Stop();
        delay(100);  // Pause for sensor reading
        
        // If we've been rotating for too long, give up
        if (millis() - stateStartTime > 10000) {  // 10 seconds timeout
          motor.Stop();
          cubePushState = STOP_COMPLETE;
          Serial1.println(F("Object not found after full rotation, operation complete"));
        }
      }
      break;
      
    case PUSH_OBJECT:
      // Push cube while monitoring for black line or 40cm distance
      if (stateStartTime == 0) {
        stateStartTime = millis();
        motor.resetEncoders();
        Serial1.println(F("Pushing object..."));
      }
      
      // Check for black line detection
      blackLineDetected = false;
      for (int i = 0; i < 5; i++) {
        if (detectColorInt(i) == COLOR_BLACK) {
          blackLineDetected = true;
          break;
        }
      }
      
      // Check distance traveled while pushing
      pushDistance = motor.getDistanceTraveled();
      
      if (blackLineDetected) {
        motor.Stop();
        cubePushState = STOP_COMPLETE;
        Serial1.println(F("Black line detected - stopping push"));
      } else if (pushDistance >= 40.0) {
        motor.Stop();
        cubePushState = STOP_COMPLETE;
        Serial1.println(F("40cm push distance reached - stopping"));
      } else {
        // Continue pushing while adjusting direction based on proximity sensors
        if (proximitysensor.strongObjectDetectedFront()) {
          motor.GaVooruit(150);  // Push forward
        } else {
          // Object lost, try to find it again
          objectStrength = proximitysensor.scanForObject();
          if (objectStrength > 4) {
            // Determine which direction based on sensor readings
            // For simplicity, just continue forward since we detected something
            motor.GaVooruit(150);  // Push forward
          } else {
            // Object completely lost
            motor.Stop();
            cubePushState = STOP_COMPLETE;
            Serial1.println(F("Object lost during push"));
          }
        }
      }
      break;
      
    case STOP_COMPLETE:
      motor.Stop();
      cubePushingMode = false;
      Serial1.println(F("Cube pushing operation completed"));
      break;
  }
}

void testEncoders() {
  Serial1.println(F("=== ENCODER TEST ==="));
  Serial1.println(F("Testing encoder functionality - driving 10cm forward"));
  
  motor.Stop();
  motor.resetEncoders();
  
  Serial1.println(F("Starting test drive..."));
  unsigned long startTime = millis();
  
  motor.GaVooruit(150);
  
  while (motor.getDistanceTraveled() < 10.0 && millis() - startTime < 10000) {
    // Print encoder values every 500ms
    if ((millis() - startTime) % 500 < 50) {
      Serial1.print(F("Left encoder: "));
      Serial1.print(motor.getLeftEncoderCount());
      Serial1.print(F(", Right encoder: "));
      Serial1.print(motor.getRightEncoderCount());
      Serial1.print(F(", Distance: "));
      Serial1.print(motor.getDistanceTraveled());
      Serial1.println(F(" cm"));
    }
    delay(50);
  }
  
  motor.Stop();
  
  Serial1.println(F("=== TEST COMPLETE ==="));
  Serial1.print(F("Final left encoder: "));
  Serial1.println(motor.getLeftEncoderCount());
  Serial1.print(F("Final right encoder: "));
  Serial1.println(motor.getRightEncoderCount());
  Serial1.print(F("Final distance: "));
  Serial1.print(motor.getDistanceTraveled());
  Serial1.println(F(" cm"));
  
  if (motor.getDistanceTraveled() < 1.0) {
    Serial1.println(F("WARNING: Encoders may not be working properly!"));
  } else {
    Serial1.println(F("Encoders appear to be working correctly"));
  }
}

void displayCommands() {
  Serial1.println(F("=== HHS-ZUMO ROBOT COMMANDS ==="));
  Serial1.println(F("CALIBRATION & SETUP:"));
  Serial1.println(F("  'c' - Calibrate colors (black/green/brown/white)"));
  Serial1.println(F("  'v' - View/display calibration values"));
  Serial1.println(F("  't' - Test encoders (drive 10cm)"));
  Serial1.println();
  
  Serial1.println(F("AUTONOMOUS MODES:"));
  Serial1.println(F("  'k' - Start line following"));
  Serial1.println(F("  'p' - Start cube pushing mode"));
  Serial1.println(F("  'o' - Stop all operations"));
  Serial1.println();
  
  Serial1.println(F("MANUAL CONTROL (when not in auto mode):"));
  Serial1.println(F("  'w' - Move forward"));
  Serial1.println(F("  's' - Move backward"));
  Serial1.println(F("  'a' - Turn left"));
  Serial1.println(F("  'd' - Turn right"));
  Serial1.println(F("  'x' - Stop motors"));
  Serial1.println();
  
  Serial1.println(F("HELP & DIAGNOSTICS:"));
  Serial1.println(F("  'q' - Show this command list"));
  Serial1.println(F("  'f' - Send diagnostics (via XBee)"));
  Serial1.println();
  
  Serial1.println(F("BUZZER SOUNDS (number keys):"));
  Serial1.println(F("  '1' - Startup sound"));
  Serial1.println(F("  '2' - Victory fanfare"));
  Serial1.println(F("  '3' - Error sound"));
  Serial1.println(F("  '4' - Siren"));
  Serial1.println(F("  '5' - Imperial March"));
  Serial1.println(F("  '6' - R2-D2 beeps"));
  Serial1.println(F("  '7' - R2-D2 scream"));
  Serial1.println(F("  '8' - Tetris theme"));
  Serial1.println(F("  '9' - Mario theme"));
  Serial1.println(F("================================"));
}