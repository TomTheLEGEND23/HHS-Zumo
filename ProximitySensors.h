#ifndef PROXIMITYSENSORS_H
#define PROXIMITYSENSORS_H

#include <Arduino.h>
#include <Zumo32U4.h>

/**
 * @class ProximitySensors
 * @brief Provides interface for reading and interpreting front proximity sensors on the Zumo32U4 robot.
 *
 * This class uses the Zumo32U4ProximitySensors library to detect objects in front of the robot.
 */
class ProximitySensors {
private:
  Zumo32U4ProximitySensors zumoProxSensors; ///< Internal object for handling Zumo's proximity sensors.
  int detectionThreshold = 4;               ///< Threshold value to determine if an object is detected.

public:
  /**
   * @brief Constructor that initializes the front proximity sensors.
   */
  ProximitySensors();

  /**
   * @brief Checks whether an object is detected in front of the robot.
   * 
   * @return true if the front left or front right proximity sensor reading exceeds the threshold.
   * @return false otherwise.
   */
  bool objectDetectedFront();

  /**
   * @brief Prints the raw front proximity sensor readings to the serial monitor.
   */
  void printReadings();
};

#endif // PROXIMITYSENSORS_H
