/**
 * @file HHS-Zumo.ino
 * @brief Main Arduino sketch for the Zumo robot, utilizing the XBee class for communication and control.
 */

#include "XBee.h"

/**
 * @brief Global XBee object for communication and control.
 */
XBee xbee;

/**
 * @brief Arduino setup function.
 *
 * Initializes serial communication for debugging and the XBee module.
 */
void setup() {
  Serial.begin(9600); /**< Initialize Serial for local debugging. */
  while (!Serial); /**< Wait for Serial to connect. */

  xbee.initializeSerial(); /**< Initialize XBee serial communication. */
  Serial.println("XBee Serial Initialized.");

  Serial.println("Setup complete. Listening for remote commands...");
}

/**
 * @brief Arduino loop function.
 *
 * Continuously checks for incoming characters from the remote XBee module
 * and processes them using the XBee library.
 */
void loop() {
  if (Serial1.available()) { /**< Check for incoming characters from the remote XBee. */
    char receivedChar = Serial1.read(); /**< Read the received character. */
    Serial.print("Received from remote: "); /**< Print each received character for debugging. */
    Serial.println(receivedChar);
    xbee.receiveCharacter(receivedChar); /**< Pass character to XBee library for processing. */
  }
}