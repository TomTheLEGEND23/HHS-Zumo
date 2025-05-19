#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;

#define NUM_SENSORS 5 // Define the number of sensors
#define BLACK_LINE_THRESHOLD 800 // Adjust this threshold based on calibration

void setup()
{
  Serial.begin(9600);
  lineSensors.initFiveSensors(); // Initialize the line sensors
  motors.setSpeeds(0, 0); // Stop the motors
}

bool isBlackLineDetected()
{
  uint16_t lineSensorValues[NUM_SENSORS];
  lineSensors.read(lineSensorValues); // Read raw sensor values

  for (uint8_t i = 0; i < NUM_SENSORS; i++)
  {
    if (lineSensorValues[i] > BLACK_LINE_THRESHOLD)
    {
      return true; // Black line detected
    }
  }
  return false; // No black line detected
}

void loop()
{
  // Read the line sensor values
  uint16_t lineSensorValues[NUM_SENSORS];
  lineSensors.read(lineSensorValues); // Read raw sensor values

  // Print the sensor values for debugging
  Serial.print("Sensor Values: ");
  for (uint8_t i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(lineSensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Check if a black line is detected
  if (isBlackLineDetected())
  {
    motors.setSpeeds(0, 0); // Stop the motors if a black line is detected
    Serial.println("Black line detected! Stopping motors.");
  }
  else
  {
    motors.setSpeeds(200, 200); // Move forward if no black line is detected
    Serial.println("No black line detected. Moving forward.");
  }

  delay(100); // Delay for stability
}

