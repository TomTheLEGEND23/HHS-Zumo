#include "Motor.h"

Motoren::Motoren():status(true), staticSpeed(0) {
}

void Motoren::Beweeg() {
  SpeedL = staticSpeed;
  SpeedR = staticSpeed;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::turn(int l, int r) {
  SpeedL = l;
  SpeedR = r;
  Motor.setSpeeds(SpeedL, SpeedR);
}

void Motoren::rotateLeft90() {
  turn(-250, 250);
  int rotation = GetEncoderRight() + 350;
  while (rotation > GetEncoderRight()) {}
  Stop();
}

void Motoren::rotateRight90() {
  turn(250, -250);
  int rotation = GetEncoderLeft() + 350;
  while (rotation > GetEncoderLeft()) {}
  Stop();
}

void Motoren::Stop() {
  staticSpeed = 0;
  Motor.setSpeeds(0, 0);
}


void Motoren::SetSpeed(int speed) {
  SpeedL = speed;
  SpeedR = speed;
  staticSpeed = speed;
}

int Motoren::GetSpeed() {
  return staticSpeed;
}

int Motoren::GetEncoderRight() {
  return encodertje.getCountsRight();
}

int Motoren::GetEncoderLeft() {
  return encodertje.getCountsLeft();
}

void Motoren::driveDistance(float distanceMM, int speed) {
  // Reset encoder counts
  encodertje.getCountsAndResetLeft();
  encodertje.getCountsAndResetRight();
  
  // Calculate target counts
  long targetCounts = abs(distanceMM * COUNTS_PER_MM);
  
  // Determine direction and set speeds
  if (distanceMM > 0) {
    // Drive forward
    turn(speed, speed);
  } else {
    // Drive backward
    turn(-speed, -speed);
  }
  
  // Drive until target distance is reached
  while (true) {
    long leftCounts = abs(GetEncoderLeft());
    long rightCounts = abs(GetEncoderRight());
    
    // Use average of both encoders
    long averageCounts = (leftCounts + rightCounts) / 2;
    
    if (averageCounts >= targetCounts) {
      break;
    }
    
    delay(10); // Small delay to prevent overwhelming the processor
  }
  
  // Stop motors
  Stop();
}