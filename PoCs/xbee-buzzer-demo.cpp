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
    // initialize Serial1 (Xbee):
    Serial1.begin(9600);
    // Reserve memory for inutString
    inputString.reserve(200);
}

void loop() {
    // pause buzzer if input equals "Pause"
    if (stringComplete.equals = "Pause") {
        buzzerRunning = false;
    }
    else if (stringComplete.equals = "Play") {
        buzzerRunning = true;
    };
    if (buzzerRunning = true) {
        // Start playing a tone with frequency 440 Hz at maximum
        // volume (15) for 200 milliseconds.
        buzzer.playFrequency(440, 200, 15);

        // Delay to give the tone time to finish.
        delay(1000);

        // Start playing note A in octave 4 at maximum volume
        // volume (15) for 2000 milliseconds.
        buzzer.playNote(NOTE_A(4), 2000, 15);

        // Wait for 200 ms and stop playing note.
        delay(200);
        buzzer.stopPlaying();

        delay(1000);

        // Start playing a fugue from program space.
        buzzer.playFromProgramSpace(fugue);

        // Wait until it is done playing.
        while(buzzer.isPlaying()){ }

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
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
