/**
 * @file ProximitySensors.ino
 * @brief Test program for the Zumo robot's proximity sensors
 * @author Tom Steenvoorden
 * @date 2025-05-08
 */

#include <Wire.h>
#include <Zumo32U4.h>
#include "ProximitySensors.h"

/** @brief Create proximity sensor object using the custom class */
ProximitySensors proxSensors;

/**
 * @brief Setup function that runs once at the start
 * 
 * Initializes serial communication and proximity sensors
 */
void setup()
{
  // Initialize Serial for output
  Serial.begin(9600);
  while (!Serial); // Wait for serial connection
  
  // Initialize and configure the proximity sensors
  proxSensors.initSensors();  // Initialize the 3 sensors (left, front, right)
  
  // Optional: Set custom detection threshold (default is 4)
  // proxSensors.setDetectionThreshold(6);
  
  Serial.println("Proximity Sensors Test");
  Serial.println("---------------------");
}

/**
 * @brief Main loop that runs continuously
 * 
 * Reads sensors, prints values, and detects objects continuously
 */
void loop()
{
  // Read the proximity sensors
  proxSensors.read();

  // Print sensor values using your custom method
  proxSensors.printReadings();  // Changed from PrintSensorValues to printReadings
  
  // Check if objects are detected
  if (proxSensors.objectDetectedLeft()) {
    Serial.println("Object detected on left!");
  }
  
  if (proxSensors.objectDetectedFront()) {
    Serial.println("Object detected in front!");
  }
  
  if (proxSensors.objectDetectedRight()) {
    Serial.println("Object detected on right!");
  }
  
  delay(200); // Delay for readability
}