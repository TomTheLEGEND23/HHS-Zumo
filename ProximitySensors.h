/**
 * @file ProximitySensors.h
 * @brief Header for ProximitySensors class that manages the Zumo robot's proximity sensors
 * @author Tom Steenvoorden
 * @date 2025-05-08
 */

#ifndef PROXIMITY_SENSORS_H
#define PROXIMITY_SENSORS_H

#include <Arduino.h>
#include <Zumo32U4.h>

/**
 * @class ProximitySensors
 * @brief A class to interface with the Zumo robot's proximity sensors
 * 
 * This class provides methods to initialize, read, and interpret data from 
 * the three proximity sensors on the Zumo robot (left, front, right).
 * It allows for object detection with configurable thresholds.
 */
class ProximitySensors {
private:
    /** @brief The Zumo32U4 proximity sensors object */
    Zumo32U4ProximitySensors zumoProxSensors;
    
    /** @brief Default threshold value for object detection */
    int detectionThreshold = 4;

public:
    /**
     * @brief Default constructor
     */
    ProximitySensors();
    
    /**
     * @brief Destructor
     */
    ~ProximitySensors();
    
    /**
     * @brief Initializes the proximity sensors
     * 
     * Sets up the three proximity sensors on the Zumo robot
     */
    void initSensors();
    
    /**
     * @brief Reads data from all proximity sensors
     * 
     * Updates the internal sensor readings from the physical sensors
     */
    void read();
    
    /**
     * @brief Checks if an object is detected on the left side
     * 
     * @return true if an object is detected above the threshold on the left sensor
     * @return false if no object is detected or below threshold
     */
    bool objectDetectedLeft();
    
    /**
     * @brief Checks if an object is detected on the right side
     * 
     * @return true if an object is detected above the threshold on the right sensor
     * @return false if no object is detected or below threshold
     */
    bool objectDetectedRight();
    
    /**
     * @brief Checks if an object is detected in front of the robot
     * 
     * @return true if an object is detected above the threshold on the front sensor
     * @return false if no object is detected or below threshold
     */
    bool objectDetectedFront();
    
    /**
     * @brief Sets the threshold for object detection
     * 
     * @param threshold The minimum sensor reading value to consider an object detected
     */
    void setDetectionThreshold(int threshold);
    
    /**
     * @brief Gets the current detection threshold
     * 
     * @return int The current threshold value used for object detection
     */
    int getDetectionThreshold();
    
    /**
     * @brief Prints the sensor readings to the Serial monitor
     * 
     * Displays all sensor readings and detection status in a human-readable format
     */
    void printReadings();
};

#endif // PROXIMITY_SENSORS_H