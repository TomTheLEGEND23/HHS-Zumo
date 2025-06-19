#ifndef XBEE_H
#define XBEE_H

#include <Arduino.h>

/**
 * @class Xbee
 * @brief Handles serial communication and button press detection via an XBee module.
 * 
 * Reads incoming serial data from the XBee and checks if a specified button (character)
 * has been pressed.
 */
class Xbee {
public:
  /**
   * @brief Constructs the Xbee object and initializes serial communication.
   */
  Xbee();

  /**
   * @brief Reads available data from the XBee serial buffer.
   * 
   * Updates internal state with the last received character.
   */
  void update();

  /**
   * @brief Checks if a specified button (character) was pressed.
   * 
   * Case-insensitive check comparing the last received character to the provided one.
   * 
   * @param c The character to check for (button).
   * @return true if the button character was received.
   * @return false otherwise.
   */
  bool isButtonPressed(char c);

private:
  char lastChar;   ///< Last character read from serial.
  bool available;  ///< Flag indicating if a new character is available.
};

#endif // XBEE_H
