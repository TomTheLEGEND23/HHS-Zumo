#include "XBee.h"

// Constructor
XBee::XBee() : lineFollowingProgramRunning(false) {
    // Initialize Zumo control objects here if they were present
    // motors = new ZumoMotors();
    // buzzer = new ZumoBuzzer();
    // display = new ZumoDisplay();
    // inputString.reserve(32); // Commented out as Arduino String handles memory dynamically
}

// Initialize serial connection from onboard Xbee module to remote "RC" module for remote control and diagnostic information.
void XBee::initializeSerial() {
    Serial1.begin(19200);
}

// Method to receive a single character from XBee
void XBee::receiveCharacter(char c) {
    // Process the character immediately as a command
    inputString = c; // Store the single character as the input string
    processReceivedData(); // Process the command immediately
}

// Method to process the accumulated inputString
void XBee::processReceivedData() {
    if (inputString.length() == 0) { // Changed from inputString.empty()
        return;
    }

    // For simplicity, we process the first character as a command.
    // More complex parsing can be implemented if needed (e.g., multi-char commands or commands with arguments).
    char command = inputString[0];

    if ((command == 'P') || (command == 'p')) { // Start Program
        startProgram();
    } else if ((command == 'O' ) || (command == 'o')) { // Stop Program (Using 'O' for Off, as 'S' is for South/Backward and typical for gaming inspired controls)
        stopProgram();
    } else if ((command == 'W') || (command == 'w')) { // Move Forward
        if (!lineFollowingProgramRunning) moveForward();
    } else if ((command == 'S') || (command == 's')) { // Move Backward
        if (!lineFollowingProgramRunning) moveBackward();
    } else if ((command == 'A') || (command == 'a')) { // Turn Left
        if (!lineFollowingProgramRunning) turnLeft();
    } else if ((command == 'D') || (command == 'd')) { // Turn Right
        if (!lineFollowingProgramRunning) turnRight();
    } else if ((command == 'X') || (command == 'x')) { // Send Diagnostics
        sendDiagnostics();
    } else {
        // Unknown command
        Serial.print("Unknown command: ");
        Serial.println(command); // Replaced std::cout with Serial.print/Serial.println
        Serial1.print("Unknown command: ");
        Serial1.println(command); // Replaced std::cout with Serial.print/Serial.println
    }

    clearInputString(); // Clear buffer after processing
}

// Method to start the Zumo program
void XBee::startProgram() {
    lineFollowingProgramRunning = true;
    Serial.println("Program started."); // Replaced std::cout with Serial.println
    Serial1.println("Program started.");
}

// Method to stop the Zumo program
void XBee::stopProgram() {
    lineFollowingProgramRunning = false;
    Serial.println("Program stopped."); // Replaced std::cout with Serial.println
    Serial1.println("Program stopped.");
}

// Method to move the Zumo forward
void XBee::moveForward() {
    Serial.println("Moving forward."); // Replaced std::cout with Serial.println
    Serial1.println("Moving forward.");
}

// Method to move the Zumo backward
void XBee::moveBackward() {
    Serial.println("Moving backward."); // Replaced std::cout with Serial.println
    Serial1.println("Moving backward.");
}

// Method to turn the Zumo left
void XBee::turnLeft() {
    Serial.println("Turning left."); // Replaced std::cout with Serial.println
    Serial1.println("Turning left.");
}

// Method to turn the Zumo right
void XBee::turnRight() {
    Serial.println("Turning right."); // Replaced std::cout with Serial.println
    Serial1.println("Turning right.");
}

// Method to send diagnostic info back to the remote XBee
void XBee::sendDiagnostics() {
    // Ensure Serial1 is initialized in your setup code, e.g., Serial1.begin(9600);
    
    Serial1.println("--- Zumo Diagnostics ---");
    Serial1.print("Program Running: ");
    Serial1.println(lineFollowingProgramRunning ? "Yes" : "No");
    Serial1.print("Input Buffer: ");
    Serial1.println(inputString.c_str()); // Send the content of std::string
    // Add more diagnostic info as needed (battery level (if possible), sensor readings from proximity sensor, line sensor, IMU, etc.)
    // For example:
    // Serial1.print("Battery Voltage: ");
    // Serial1.println(readBatteryMillivolts());
    Serial1.println("--- End Diagnostics ---");
}

// Helper to clear the input string
void XBee::clearInputString() {
    inputString = ""; // Changed from inputString.clear()
}

