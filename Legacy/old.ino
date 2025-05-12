// Non-working test code!!

#include <XBee.h>

// Global XBee object
XBee xbee;

void setup() {
  // Initialize Serial for local debugging (optional)
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to connect (useful for some boards like Leonardo)
  Serial.println("Zumo XBee Communication Test Sketch");

  // Initialize XBee serial communication
  xbee.initializeSerial();
  Serial.println("XBee Serial Initialized.");

  // Send initial diagnostics
  Serial.println("Sending initial diagnostics...");
  xbee.sendDiagnostics();

  // Test some state changes and send diagnostics after each
  Serial.println("Testing startProgram()...");
  xbee.startProgram();
  xbee.sendDiagnostics();
  delay(1000); // Wait a bit

  Serial.println("Testing stopProgram()...");
  xbee.stopProgram();
  xbee.sendDiagnostics();
  delay(1000);

  // Simulate some movement commands - these won't actually move the Zumo
  // in this test if motors are not connected/controlled by these placeholder functions,
  // but we can check if the XBee class processes them.
  Serial.println("Simulating moveForward()...");
  xbee.receiveCharacter('W'); // Simulate receiving 'W' command
  // processReceivedData is called internally by receiveCharacter if it's a newline,
  // or we can call it directly if we accumulate commands differently.
  // For this test, let's assume single char commands are processed upon receipt if newline is sent.
  // To ensure processing for this test, we can send a newline or call processReceivedData.
  // xbee.processReceivedData(); // If commands are not newline terminated in your actual remote.
  xbee.sendDiagnostics(); // Check state if any internal state was supposed to change
  delay(1000);

  Serial.println("Simulating turnLeft()...");
  xbee.receiveCharacter('A');
  xbee.sendDiagnostics();
  delay(1000);

  Serial.println("Setup complete. Listening for remote commands...");
}

void loop() {
  // Check for incoming characters from the remote XBee (Serial1)
  if (Serial1.available()) {
    char receivedChar = Serial1.read();
    Serial.print("Received from remote: ");
    Serial.println(receivedChar);
    xbee.receiveCharacter(receivedChar); // Pass character to XBee library

    // Optional: Send diagnostics immediately after processing a command
    // This might be too chatty depending on command frequency.
    // xbee.sendDiagnostics();
  }

  // Periodically send diagnostics to the remote, e.g., every 5 seconds
  static unsigned long lastDiagTime = 0;
  if (millis() - lastDiagTime > 5000) {
    Serial.println("Sending periodic diagnostics...");
    xbee.sendDiagnostics();
    lastDiagTime = millis();
  }

  // You can add other test calls here, for example, to simulate commands
  // that would be sent from the remote XBee.
  // For example, to test the 'P' command (startProgram)
  // xbee.receiveCharacter('P');
  // xbee.receiveCharacter('\n'); // Assuming newline terminates commands

  // Add a small delay to keep the loop from running too fast
  delay(100);
}

// Note: This sketch assumes that the XBee.h and XBee.cpp files are
// accessible in the Arduino IDE's library path or in the same sketch folder.
// For Zumo specific functionalities like actual motor movement, buzzer sounds, etc.,
// you would need to uncomment and implement the Zumo specific classes (ZumoMotors, ZumoBuzzer)
// and their corresponding methods within your XBee class.
// This sketch primarily focuses on testing the communication flow and diagnostic reporting.
