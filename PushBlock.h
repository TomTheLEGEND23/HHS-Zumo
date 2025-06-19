#ifndef PUSHBLOCK_H
#define PUSHBLOCK_H

#include <Arduino.h>

/**
 * @class PushBlock
 * @brief Controls a behavior where the robot detects, aligns with, and pushes a block.
 *
 * This class implements a multi-phase state machine:
 * - Positioning the robot
 * - Rotating to detect and track an object
 * - Calculating the center of the object via timed rotation
 * - Pushing the object forward
 * - Stopping once a line is detected
 */
class PushBlock {
public:
  /**
   * @brief Constructor that initializes the internal state of the PushBlock sequence.
   */
  PushBlock();

  /**
   * @brief Executes the logic for detecting and pushing a block.
   * 
   * This function controls the entire push sequence: initial positioning, rotation to detect
   * the object, re-alignment, pushing forward, and stopping upon line detection.
   *
   * @param BASE_SPEED The base speed at which the robot moves when pushing.
   */
  void pushBlock(int BASE_SPEED);

  /**
   * @brief Checks if the push block behavior has completed.
   * 
   * @return true if the object was pushed and the sequence is complete.
   * @return false otherwise.
   */
  bool completedPushing();

private:
  int distance;                  ///< Target encoder distance to move.
  bool initial;                  ///< Flag for initialization step.
  bool completed;                ///< Flag indicating if push sequence is done.
  bool pushingObject;           ///< Whether the robot is currently pushing an object.

  bool rotatingToFind = true;   ///< Whether the robot is still rotating to find an object.
  bool foundObject = false;     ///< Whether the object has been detected.
  unsigned long seenStart = 0;  ///< Time when the object was first seen.
  unsigned long seenEnd = 0;    ///< Time when the object was last seen.
  bool rotatedBack = false;     ///< Whether the robot has rotated back to the object's center.
  bool readyToPush = false;     ///< Whether the robot is aligned and ready to push.
};

#endif // PUSHBLOCK_H
