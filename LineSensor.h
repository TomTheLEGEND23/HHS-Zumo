#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "WString.h"
#include <Zumo32U4.h>
#include "XBee.h"
#include "Motor.h"

/**
 * @class LineSensor
 * @brief Handles interaction with the Zumo32U4 line sensors for line detection and calibration.
 *
 * This class manages the raw and calibrated readings from the line sensors and provides methods
 * to calibrate the sensors and determine the position of a detected line.
 */
class LineSensor {
public:
  /**
   * @brief Constructs a LineSensor object and initializes the sensors.
   */
  LineSensor();

  /**
   * @brief Calibrates the line sensors to establish baseline minimum readings.
   *
   * This function waits for the user to press the 'c' button (via Xbee) and then reads values
   * from each sensor to determine the minimum values under ambient conditions.
   *
   * @param xbee Reference to the Xbee object for receiving start command.
   * @param motors Reference to the motor object, if motor control is needed during calibration.
   */
  void calibrateLineSensor(Xbee &xbee, Motoren &motors);

  /**
   * @brief Calculates and returns the line position from sensor data.
   *
   * Computes a weighted average of the line position between a specified range of sensors.
   * Returns -1 if no line is detected.
   *
   * @param l Left-most sensor index to consider (inclusive).
   * @param r Right-most sensor index to consider (exclusive).
   * @return Position estimate from 0 to 4000, or -1 if line not detected.
   */
  int detectedLine(int l, int r);

  /**
   * @brief Gets the raw reading from a specific sensor.
   *
   * @param l Index of the sensor (0 to 4).
   * @return Raw sensor value.
   */
  int giveRawValue(int l);

  /**
   * @brief Returns the calibrated value of a specific sensor.
   *
   * @param l Index of the sensor (0 to 4).
   * @return Calibrated sensor value (raw - minimum).
   */
  int giveCalValue(int l);

  /**
   * @brief Updates the provided array with calibrated sensor readings.
   *
   * Subtracts the minimum detection values from the raw readings.
   *
   * @param calValues Array of size 5 to store calibrated values.
   */
  void updateSensors(int calValues[5]);

private:
  /**
   * @brief Stores the most recent raw sensor values.
   */
  unsigned int linesensorRawValue[5];

  /**
   * @brief Stores the minimum values detected during calibration.
   */
  unsigned int MinimumDetection[5]{ 1023, 1023, 1023, 1023, 1023 };

  /**
   * @brief Zumo32U4LineSensors object for accessing hardware sensor functions.
   */
  Zumo32U4LineSensors zumoLineSensor;
};

#endif // LINESENSOR_H
