#ifndef MOTOR_H
#define MOTOR_H

#include <Zumo32U4.h>

/**
 * @class Motoren
 * @brief Controls the movement of the robot using Zumo32U4 motors and encoders.
 *
 * This class provides functions to move the robot, turn, stop, rotate by 90 degrees,
 * and interact with motor speed and encoder values.
 */
class Motoren {
public:
  /**
   * @brief Constructor for the Motoren class. Initializes motor status and speed.
   */
  Motoren();

  /**
   * @brief Moves the robot forward at the set static speed.
   */
  void Beweeg();

  /**
   * @brief Sets the motor speeds to the given left and right values.
   * 
   * @param l Speed for the left motor.
   * @param r Speed for the right motor.
   */
  void turn(int l, int r);

  /**
   * @brief Stops the robot by setting motor speeds to zero.
   */
  void Stop();

  /**
   * @brief Sets a uniform speed for both motors and stores it as static speed.
   * 
   * @param speed The speed to be applied to both motors.
   */
  void SetSpeed(int speed);

  /**
   * @brief Rotates the robot approximately 90 degrees to the left.
   */
  void rotateLeft90();

  /**
   * @brief Rotates the robot approximately 90 degrees to the right.
   */
  void rotateRight90();

  /**
   * @brief Gets the current static speed.
   * 
   * @return The stored static speed.
   */
  int GetSpeed();

  /**
   * @brief Returns the encoder count of the left motor.
   * 
   * @return Encoder count from the left motor.
   */
  int GetEncoderLeft();

  /**
   * @brief Returns the encoder count of the right motor.
   * 
   * @return Encoder count from the right motor.
   */
  int GetEncoderRight();

  /**
   * @brief Resets the encoder counts for both motors.
   */
  void ResetEncoder();

private:
  Zumo32U4Motors Motor;         ///< Zumo motor driver object.
  Zumo32U4Encoders encodertje;  ///< Zumo encoder object.

  bool status;        ///< Indicates whether the motor system is operational.
  int staticSpeed;    ///< Stored speed used when calling Beweeg().
  int SpeedL;         ///< Last set speed for the left motor.
  int SpeedR;         ///< Last set speed for the right motor.
};

#endif // MOTOR_H
