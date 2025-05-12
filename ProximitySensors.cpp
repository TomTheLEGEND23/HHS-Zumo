/**
 * @file ProximitySensors.cpp
 * @brief Implementation of the ProximitySensors class for Zumo robot
 * @author Tom Steenvoorden
 * @date 2025-05-08
 */

#include "ProximitySensors.h"

/**
 * @brief Default constructor for ProximitySensors class
 */
ProximitySensors::ProximitySensors() {
    // Default constructor
}

/**
 * @brief Destructor for ProximitySensors class
 */
ProximitySensors::~ProximitySensors() {
    // Destructor - nothing to clean up
}

/**
 * @brief Initializes the proximity sensors
 * 
 * Sets up the three proximity sensors on the Zumo robot
 */
void ProximitySensors::initSensors() {
    zumoProxSensors.initThreeSensors();
}

/**
 * @brief Reads data from all proximity sensors
 * 
 * Updates the internal sensor readings from the physical sensors
 */
void ProximitySensors::read() {
    zumoProxSensors.read();
}

/**
 * @brief Checks if an object is detected on the left side
 * 
 * @return true if an object is detected above the threshold on the left sensor
 * @return false if no object is detected or below threshold
 */
bool ProximitySensors::objectDetectedLeft() {
    return (zumoProxSensors.countsLeftWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsLeftWithRightLeds() > detectionThreshold);
}

/**
 * @brief Checks if an object is detected on the right side
 * 
 * @return true if an object is detected above the threshold on the right sensor
 * @return false if no object is detected or below threshold
 */
bool ProximitySensors::objectDetectedRight() {
    return (zumoProxSensors.countsRightWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsRightWithRightLeds() > detectionThreshold);
}

/**
 * @brief Checks if an object is detected in front of the robot
 * 
 * @return true if an object is detected above the threshold on the front sensor
 * @return false if no object is detected or below threshold
 */
bool ProximitySensors::objectDetectedFront() {
    return (zumoProxSensors.countsFrontWithLeftLeds() > detectionThreshold) || 
           (zumoProxSensors.countsFrontWithRightLeds() > detectionThreshold);
}

/**
 * @brief Sets the threshold for object detection
 * 
 * @param threshold The minimum sensor reading value to consider an object detected
 */
void ProximitySensors::setDetectionThreshold(int threshold) {
    detectionThreshold = threshold;
}

/**
 * @brief Gets the current detection threshold
 * 
 * @return int The current threshold value used for object detection
 */
int ProximitySensors::getDetectionThreshold() {
    return detectionThreshold;
}

/**
 * @brief Prints the sensor readings to the Serial monitor
 * 
 * Displays all sensor readings and detection status in a human-readable format.
 * Format: L: left_left/left_right  F: front_left/front_right  R: right_left/right_right
 * Also shows whether objects are detected (Y/N) for each direction
 */
void ProximitySensors::printReadings() {
    Serial.print("L: ");
    Serial.print(zumoProxSensors.countsLeftWithLeftLeds());
    Serial.print("/");
    Serial.print(zumoProxSensors.countsLeftWithRightLeds());
    
    Serial.print("\tF: ");
    Serial.print(zumoProxSensors.countsFrontWithLeftLeds());
    Serial.print("/");
    Serial.print(zumoProxSensors.countsFrontWithRightLeds());
    
    Serial.print("\tR: ");
    Serial.print(zumoProxSensors.countsRightWithLeftLeds());
    Serial.print("/");
    Serial.println(zumoProxSensors.countsRightWithRightLeds());
    
    Serial.print("Detection: L:");
    Serial.print(objectDetectedLeft() ? "Y" : "N");
    Serial.print(" F:");
    Serial.print(objectDetectedFront() ? "Y" : "N");
    Serial.print(" R:");
    Serial.println(objectDetectedRight() ? "Y" : "N");
}