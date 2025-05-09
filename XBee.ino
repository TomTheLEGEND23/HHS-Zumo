#include "XBee.h"

// Global XBee object
XBee xbee;

void setup() {
  // Initialize Serial for local debugging
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to connect

  // Initialize XBee serial communication
  xbee.initializeSerial();
  Serial.println("XBee Serial Initialized.");

  Serial.println("Setup complete. Listening for remote commands...");
}

void loop() {
  // Check for incoming characters from the remote XBee (Serial1)
  if (Serial1.available()) {
    char receivedChar = Serial1.read();
    Serial.print("Received from remote: "); // Optional: print each char as it arrives
    Serial.println(receivedChar);
    xbee.receiveCharacter(receivedChar); // Pass character to XBee library
  }
  // No delay needed here as Serial1.available() is non-blocking
  // and XBee.receiveCharacter() handles command processing on newline.
}

// Note: This sketch assumes that the XBee.h and XBee.cpp files are
// accessible in the Arduino IDE's library path or in the same sketch folder.