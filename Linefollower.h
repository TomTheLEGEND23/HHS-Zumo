#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

#include <Arduino.h>
#include "Motor.h"
#include "LineSensor.h"

/**
 * @class LineFollower
 * @brief Handles line-following logic using line sensors and motor control.
 *
 * Detects black lines on various surfaces, identifies gray and brown zones,
 * and performs turns or recovery behavior accordingly. Supports corner detection
 * and proportional control to follow a line.
 */
class LineFollower {
public:
  /**
   * @brief Constructs a LineFollower object.
   */
  LineFollower();

  /**
   * @brief Executes the main line-following logic.
   *
   * Uses line sensors to follow a black line, detect special zones (gray/brown),
   * and correct course based on proportional control.
   *
   * @param motor Reference to Motoren controller.
   * @param linesensor Reference to LineSensor object.
   * @param BASE_SPEED Base movement speed for line following.
   * @param MAX_SPEED Maximum speed to apply with correction.
   * @param KP Proportional gain constant for line correction.
   */
  void FollowLine(Motoren &motor, LineSensor &linesensor, const int BASE_SPEED, const int MAX_SPEED, const float KP);

  /**
   * @brief Returns whether the left sensor detected a gray surface.
   * @return True if left gray zone detected.
   */
  bool GetLeftGray();

  /**
   * @brief Returns whether the right sensor detected a gray surface.
   * @return True if right gray zone detected.
   */
  bool GetRightGray();

  /**
   * @brief Sets the left gray detection flag.
   * @param i Boolean value (0 or 1) to set the flag.
   */
  void SetLeftGray(int i);

  /**
   * @brief Sets the right gray detection flag.
   * @param i Boolean value (0 or 1) to set the flag.
   */
  void SetRightGray(int i);

  /**
   * @brief Returns whether the left sensor detected a brown surface.
   * @return True if left brown zone detected.
   */
  bool GetLeftBrown();

  /**
   * @brief Returns whether the right sensor detected a brown surface.
   * @return True if right brown zone detected.
   */
  bool GetRightBrown();

  /**
   * @brief Sets the left brown detection flag.
   * @param i Boolean value (0 or 1) to set the flag.
   */
  void SetLeftBrown(int i);

  /**
   * @brief Sets the right brown detection flag.
   * @param i Boolean value (0 or 1) to set the flag.
   */
  void SetRightBrown(int i);

private:
  /** @brief Flag indicating whether a left corner is being detected. */
  bool onLeftCorner = false;

  /** @brief Flag indicating whether a right corner is being detected. */
  bool onRightCorner = false;

  /** @brief Timestamp of last detected left corner. */
  unsigned long leftCornerTime = 0;

  /** @brief Timestamp of last detected right corner. */
  unsigned long rightCornerTime = 0;

  /** @brief Timestamp of last left peak value. */
  unsigned long leftlastPeakTime = 0;

  /** @brief Timestamp of last right peak value. */
  unsigned long rightlastPeakTime = 0;

  /** @brief Index of the right sensor used for line detection. */
  int rightSensor = 4;

  /** @brief Index of the left sensor used for line detection. */
  int leftSensor = 1;

  /** @brief Highest detected left sensor value during gray/brown detection. */
  int leftpeakValue;

  /** @brief Last stored left peak value. */
  int leftlastPeak;

  /** @brief Flag to track rising edge detection on left sensor. */
  bool leftedgeDetection = false;

  /** @brief Highest detected right sensor value during gray/brown detection. */
  int rightpeakValue;

  /** @brief Last stored right peak value. */
  int rightlastPeak;

  /** @brief Flag to track rising edge detection on right sensor. */
  bool rightedgeDetection = false;

  /** @brief True if gray surface detected on left side. */
  bool LeftGray = false;

  /** @brief True if gray surface detected on right side. */
  bool RightGray = false;

  /** @brief True if brown surface detected on left side. */
  bool LeftBrown = false;

  /** @brief True if brown surface detected on right side. */
  bool RightBrown = false;
};

#endif // LINEFOLLOWER_H
