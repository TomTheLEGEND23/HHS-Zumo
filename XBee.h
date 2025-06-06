/**
 * @file XBee.h
 * @brief Header file for the XBee class, which handles communication and control for the Zumo robot using XBee.
 */

#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>
#include <Zumo32U4.h>

/**
 * @class XBee
 * @brief A class to handle communication and control for the Zumo robot using XBee.
 *
 * The XBee class provides methods to initialize serial communication, process commands,
 * and control the Zumo robot's movements and diagnostics.
 */
class XBee {
public:
    /**
     * @brief Constructor for the XBee class.
     *
     * Initializes the XBee object and sets default states.
     */
    XBee();

    /**
     * @brief Initializes the serial connection for the XBee module.
     *
     * Sets up the serial communication with the remote XBee module.
     */
    void initializeSerial();

    /**
     * @brief Receives a single character from the XBee module.
     *
     * @param c The character received from the XBee module.
     */
    void receiveCharacter(char c);

    /**
     * @brief Processes the accumulated input string.
     *
     * Interprets the input string as commands and executes the corresponding actions.
     */
    void processReceivedData();

    /**
     * @brief Sends diagnostic information to the remote XBee module.
     *
     * Provides details about the current state of the Zumo robot.
     */
    void sendDiagnostics();

    /**
     * @brief Clears the input string buffer.
     */
    void clearInputString();
    
    /**
     * @brief Updates XBee communication and processes incoming data.
     */
    void update();
    
    /**
     * @brief Checks if a specific button/command was pressed.
     */
    bool isButtonPressed(char command);
    
    /**
     * @brief Sets the line following program state.
     */
    void setLineFollowingProgram(bool state);
    
    /**
     * @brief Gets the line following program state.
     */
    bool isLineFollowingProgramRunning();

private:
    /**
     * @brief Stores incoming characters from the XBee module.
     */
    String inputString;

    /**
     * @brief Tracks whether the line-following program is active.
     */
    bool lineFollowingProgramRunning;
    
    /**
     * @brief Tracks the last pressed button/command.
     */
    char lastButtonPressed;
    
    /**
     * @brief Flag to check if button was pressed this cycle.
     */
    bool buttonPressedFlag;
};

#endif // XBEE_H