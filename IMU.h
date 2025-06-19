#ifndef IMU_H
#define IMU_H

#include "Arduino.h"
#include <Zumo32U4.h>
#include <Wire.h>

/**
 * @class IMU
 * @brief Interface for interacting with a Zumo32U4 IMU sensor.
 *
 * This class provides initialization, calibration, and access to
 * corrected accelerometer and gyroscope readings. It also computes
 * pitch and roll angles based on accelerometer data.
 */
class IMU {
public:
  /**
   * @brief Constructs an IMU object with zeroed biases and uninitialized status.
   */
  IMU();

  /**
   * @brief Initializes the IMU sensor and computes bias values.
   * @return True if initialization is successful, false otherwise.
   */
  bool init();

  /**
   * @brief Returns corrected acceleration along the X-axis.
   * @return Acceleration in meters per second squared (m/s²).
   */
  float accelX();

  /**
   * @brief Returns corrected acceleration along the Y-axis.
   * @return Acceleration in meters per second squared (m/s²).
   */
  float accelY();

  /**
   * @brief Returns corrected acceleration along the Z-axis.
   * @return Acceleration in meters per second squared (m/s²).
   */
  float accelZ();

  /**
   * @brief Returns corrected gyroscope value along the X-axis.
   * @return Angular velocity in degrees per second (°/s).
   */
  float gyroX();

  /**
   * @brief Returns corrected gyroscope value along the Y-axis.
   * @return Angular velocity in degrees per second (°/s).
   */
  float gyroY();

  /**
   * @brief Returns corrected gyroscope value along the Z-axis.
   * @return Angular velocity in degrees per second (°/s).
   */
  float gyroZ();

  /**
   * @brief Returns whether the IMU has been successfully initialized.
   * @return True if initialized, false otherwise.
   */
  bool fetchStatus();

  /**
   * @brief Computes the pitch angle from accelerometer data.
   * @return Pitch angle in radians.
   */
  float pitch();

  /**
   * @brief Computes the roll angle from accelerometer data.
   * @return Roll angle in radians.
   */
  float roll();

private:
  /**
   * @brief IMU sensor object (Zumo32U4IMU).
   */
  Zumo32U4IMU imu;

  /**
   * @brief Indicates whether the IMU was successfully initialized.
   */
  bool status;

  /**
   * @brief Accelerometer bias along the X-axis.
   */
  float accelBiasX;

  /**
   * @brief Accelerometer bias along the Y-axis.
   */
  float accelBiasY;

  /**
   * @brief Accelerometer bias along the Z-axis.
   */
  float accelBiasZ;

  /**
   * @brief Gyroscope bias along the X-axis.
   */
  float gyroBiasX;

  /**
   * @brief Gyroscope bias along the Y-axis.
   */
  float gyroBiasY;

  /**
   * @brief Gyroscope bias along the Z-axis.
   */
  float gyroBiasZ;
};

#endif // IMU_H
