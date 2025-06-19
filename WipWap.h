#ifndef WIPWAP_H
#define WIPWAP_H

#include <Arduino.h>

/**
 * @class WipWap
 * @brief Controls a rocking (wipping) movement based on IMU pitch readings.
 * 
 * The class moves the robot forward or backward depending on the pitch angle from the IMU,
 * and detects when rocking is complete by monitoring pitch stabilization over a period.
 */
class WipWap {
public:
  /**
   * @brief Constructor initializes internal state.
   */
  WipWap();

  /**
   * @brief Executes the rocking motion logic.
   * 
   * Moves the robot forward or backward based on the IMU pitch,
   * and monitors pitch stabilization to determine when rocking is complete.
   */
  void Wippen();

  /**
   * @brief Checks whether the rocking motion has been completed.
   * 
   * @return true if the rocking motion is complete.
   * @return false otherwise.
   */
  bool completedWippen();

private:
  bool completed;  ///< Flag indicating if the rocking motion has completed.
  int distance;    ///< Target encoder count for movement during rocking.
};

#endif // WIPWAP_H
