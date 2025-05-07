// Compile in Arduino IDE. Zumo library and board need to be installed.

// Include necessary libraries to communicate with the Zumo.
#include <Wire.h>
#include <Zumo32U4.h>

String inputString = ""; // temp store for incoming data
bool stringComplete = false; 
bool buzzerRunning = false;

Zumo32U4Buzzer buzzer;

// Store this song in program space using the PROGMEM macro.
// Later we will play it directly from program space, bypassing
// the need to load it all into RAM first.
const char fugue[] PROGMEM =
  "! O5 L16 agafaea dac+adaea fa<aa<bac#a dac#adaea f"
  "O6 dcd<b-d<ad<g d<f+d<gd<ad<b- d<dd<ed<f+d<g d<f+d<gd<ad"
  "L8 MS <b-d<b-d MLe-<ge-<g MSc<ac<a ML d<fd<f O5 MS b-gb-g"
  "ML >c#e>c#e MS afaf ML gc#gc# MS fdfd ML e<b-e<b-"
  "O6 L16ragafaea dac#adaea fa<aa<bac#a dac#adaea faeadaca"
  "<b-acadg<b-g egdgcg<b-g <ag<b-gcf<af dfcf<b-f<af"
  "<gf<af<b-e<ge c#e<b-e<ae<ge <fe<ge<ad<fd"
  "O5 e>ee>ef>df>d b->c#b->c#a>df>d e>ee>ef>df>d"
  "e>d>c#>db>d>c#b >c#agaegfe f O6 dc#dfdc#<b c#4";

void setup() {
    Serial.begin(9600);      // For debugging via USB
    Serial1.begin(9600);     // XBee communication
    inputString.reserve(200);
    Serial.println("Ready.");
    ledGreen(1);
    delay(500);
    ledGreen(0);
}

void loop() {
    serialEvent();
    if (stringComplete) {
      inputString.trim();  // Removes newline, carriage return, spaces
    if (inputString == "Play") {
        buzzerRunning = true;
        Serial.println("Play received!");
    } else if (inputString == "Pause") {
        buzzerRunning = false;
        Serial.println("Pause received!");
    }
    inputString = "";
    stringComplete = false;
    };
    if (buzzerRunning == true) {
      Serial.println("Blinking LEDs...");
      // Turn the LEDs on.
      ledRed(1);
      ledYellow(1);
      ledGreen(1);

      // Wait for a second.
      delay(1000);

      // Turn the LEDs off.
      ledRed(0);
      ledYellow(0);
      ledGreen(0);

      // Wait for a second.
      delay(1000);
    }
    
}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    inputString += inChar;
    Serial.print("Received: ");
    Serial.println(inChar); // Print each character received for debugging

    if (inChar == '\n') {
      stringComplete = true;
      Serial.print("Full input received: ");
      Serial.println(inputString);
    }
  }
}
