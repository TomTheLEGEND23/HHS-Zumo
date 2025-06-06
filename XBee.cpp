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
XBee::XBee() : lineFollowingProgramRunning(false), lastButtonPressed(0), buttonPressedFlag(false) {
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
    lastButtonPressed = c; /**< Store the last button pressed. */
    buttonPressedFlag = true; /**< Set button pressed flag. */
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

    if ((command == 'F') || (command == 'f')) {
        sendDiagnostics(); /**< Send diagnostic information. */
    } else {
        // All other commands are handled by the main program
        // XBee class only handles diagnostics now
    }

    clearInputString(); /**< Clear the input string buffer after processing. */
}

/**
 * @brief Sends diagnostic information to the remote XBee module.
 *
 * Provides details about the current state of the Zumo robot.
 */
void XBee::sendDiagnostics() {
    Serial1.println(F("--- Zumo Diagnostics ---"));
    Serial1.print(F("Program Running: "));
    Serial1.println(lineFollowingProgramRunning ? F("Yes") : F("No"));
    Serial1.print(F("Input Buffer: "));
    Serial1.println(inputString.c_str()); /**< Send the content of the input string. */
    Serial1.println(F("--- End Diagnostics ---"));
}

/**
 * @brief Updates XBee communication and processes incoming data.
 */
void XBee::update() {
    // Check for incoming serial data
    if (Serial1.available()) {
        char c = Serial1.read();
        receiveCharacter(c);
    }
}

/**
 * @brief Checks if a specific button/command was pressed.
 */
bool XBee::isButtonPressed(char command) {
    if (buttonPressedFlag && (lastButtonPressed == command || lastButtonPressed == (command - 32))) {
        buttonPressedFlag = false; // Reset flag after checking
        return true;
    }
    return false;
}

/**
 * @brief Sets the line following program state.
 */
void XBee::setLineFollowingProgram(bool state) {
    lineFollowingProgramRunning = state;
}

/**
 * @brief Gets the line following program state.
 */
bool XBee::isLineFollowingProgramRunning() {
    return lineFollowingProgramRunning;
}

/**
 * @brief Clears the input string buffer.
 */
void XBee::clearInputString() {
    inputString = ""; /**< Clear the input string. */
}

