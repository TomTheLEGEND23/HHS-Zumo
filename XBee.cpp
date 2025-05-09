/**
 * @file XBee.cpp
 * @brief Implementation file for the XBee class, which handles communication and control for the Zumo robot using XBee.
 */

#include "XBee.h"

/**
 * @brief Constructor for the XBee class.
 *
 * Initializes the XBee object and sets default states.
 */
XBee::XBee() : lineFollowingProgramRunning(false) {
    // Initialization of Zumo control objects can be added here if needed.
}

/**
 * @brief Initializes the serial connection for the XBee module.
 *
 * Sets up the serial communication with the remote XBee module.
 */
void XBee::initializeSerial() {
    Serial1.begin(19200); /**< Initialize Serial1 for XBee communication. */
}

/**
 * @brief Receives a single character from the XBee module.
 *
 * @param c The character received from the XBee module.
 */
void XBee::receiveCharacter(char c) {
    inputString = c; /**< Store the single character as the input string. */
    processReceivedData(); /**< Process the command immediately. */
}

/**
 * @brief Processes the accumulated input string.
 *
 * Interprets the input string as commands and executes the corresponding actions.
 */
void XBee::processReceivedData() {
    if (inputString.length() == 0) {
        return; /**< Exit if the input string is empty. */
    }

    char command = inputString[0]; /**< Extract the first character as the command. */

    if ((command == 'P') || (command == 'p')) {
        startProgram(); /**< Start the Zumo program. */
    } else if ((command == 'O') || (command == 'o')) {
        stopProgram(); /**< Stop the Zumo program. */
    } else if ((command == 'W') || (command == 'w')) {
        if (!lineFollowingProgramRunning) moveForward(); /**< Move forward if the program is not running. */
    } else if ((command == 'S') || (command == 's')) {
        if (!lineFollowingProgramRunning) moveBackward(); /**< Move backward if the program is not running. */
    } else if ((command == 'A') || (command == 'a')) {
        if (!lineFollowingProgramRunning) turnLeft(); /**< Turn left if the program is not running. */
    } else if ((command == 'D') || (command == 'd')) {
        if (!lineFollowingProgramRunning) turnRight(); /**< Turn right if the program is not running. */
    } else if ((command == 'X') || (command == 'x')) {
        sendDiagnostics(); /**< Send diagnostic information. */
    } else {
        Serial.print("Unknown command: "); /**< Print unknown command for debugging. */
        Serial.println(command);
        Serial1.print("Unknown command: ");
        Serial1.println(command);
    }

    clearInputString(); /**< Clear the input string buffer after processing. */
}

/**
 * @brief Starts the Zumo program.
 *
 * Activates the line-following program or other functionalities.
 */
void XBee::startProgram() {
    lineFollowingProgramRunning = true; /**< Set the program running state to true. */
    Serial.println("Program started.");
    Serial1.println("Program started.");
}

/**
 * @brief Stops the Zumo program.
 *
 * Deactivates the line-following program or other functionalities.
 */
void XBee::stopProgram() {
    lineFollowingProgramRunning = false; /**< Set the program running state to false. */
    Serial.println("Program stopped.");
    Serial1.println("Program stopped.");
}

/**
 * @brief Moves the Zumo robot forward.
 */
void XBee::moveForward() {
    Serial.println("Moving forward.");
    Serial1.println("Moving forward.");
}

/**
 * @brief Moves the Zumo robot backward.
 */
void XBee::moveBackward() {
    Serial.println("Moving backward.");
    Serial1.println("Moving backward.");
}

/**
 * @brief Turns the Zumo robot left.
 */
void XBee::turnLeft() {
    Serial.println("Turning left.");
    Serial1.println("Turning left.");
}

/**
 * @brief Turns the Zumo robot right.
 */
void XBee::turnRight() {
    Serial.println("Turning right.");
    Serial1.println("Turning right.");
}

/**
 * @brief Sends diagnostic information to the remote XBee module.
 *
 * Provides details about the current state of the Zumo robot.
 */
void XBee::sendDiagnostics() {
    Serial1.println("--- Zumo Diagnostics ---");
    Serial1.print("Program Running: ");
    Serial1.println(lineFollowingProgramRunning ? "Yes" : "No");
    Serial1.print("Input Buffer: ");
    Serial1.println(inputString.c_str()); /**< Send the content of the input string. */
    Serial1.println("--- End Diagnostics ---");
}

/**
 * @brief Clears the input string buffer.
 */
void XBee::clearInputString() {
    inputString = ""; /**< Clear the input string. */
}

