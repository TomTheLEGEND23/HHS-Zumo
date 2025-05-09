#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>

// Forward declarations for Zumo control classes (not present, so commented out)
// class ZumoMotors;
// class ZumoBuzzer;
// class ZumoDisplay;

class XBee {
public:
    XBee(); // Constructor

    void initializeSerial(); // Initialize serial connection

    void receiveCharacter(char c); // Method to receive a single character from XBee
    void processReceivedData(); // Method to process the accumulated inputString

    // Methods for remote control commands
    void startProgram();
    void stopProgram();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void sendDiagnostics(); // Method to send diagnostic info back

    void clearInputString(); // Helper to clear the input string/character

private:
    String inputString; // Stores incoming characters
    bool lineFollowingProgramRunning;     // Tracks if line following program is active
};

#endif // XBEE_H