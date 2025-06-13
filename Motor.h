#ifndef MOTOR_H
#define MOTOR_H

#include <Zumo32U4.h>

// Encoder constants for distance calculation
#define COUNTS_PER_ROTATION 909.7  // CPR for 75:1 gear ratio
#define WHEEL_DIAMETER 32.0        // Wheel diameter in mm
#define COUNTS_PER_MM (COUNTS_PER_ROTATION / (PI * WHEEL_DIAMETER))

class Motoren {
public:
  Motoren();
  void Beweeg();
  void turn(int, int);
  void Stop();
  void SetSpeed(int);
  void rotateLeft90();
  void rotateRight90();
  void driveDistance(float distanceMM, int speed = 200);
  int GetSpeed();
  int GetEncoderLeft();
  int GetEncoderRight();

private:
  Zumo32U4Motors Motor;
  Zumo32U4Encoders encodertje;
  bool status;
  int staticSpeed;
  int SpeedL;
  int SpeedR;
};

#endif