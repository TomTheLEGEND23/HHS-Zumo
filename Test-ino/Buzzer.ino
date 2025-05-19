/**
 * @file Buzzer.ino
 * @brief Demo program for Zumo32U4 buzzer functionality
 * @author Tom Steenvoorden
 * @date 10 May 2025
 * 
 * This program demonstrates various sound effects and melodies
 * that can be played using the Zumo32U4 buzzer.
 * Connect to the serial monitor at 9600 baud and press the 
 * displayed keys to trigger different sounds.
 */

#include <Zumo32U4.h>
#include "Buzzer.h"

/**
 * @brief Buzzer object for controlling the Zumo32U4 buzzer
 */
Buzzer buzzer;

/**
 * @brief Setup function that runs once at program start
 * 
 * Initializes serial communication and plays a startup sound.
 */
void setup() {
  Serial.begin(9600);
  // Play a startup sound
  buzzer.playStartup();
}

/**
 * @brief Main program loop
 * 
 * Displays a menu of available sounds in the serial monitor
 * and responds to user input to play selected sounds.
 */
void loop() {
  Serial.println("---------------------------------");
  Serial.println("Zumo Buzzer Demo");
  Serial.println("Press a key to play a sound:");
  Serial.println("B: Single beep (440Hz)");
  Serial.println("M: Mario theme");
  Serial.println("S: Startup sound");
  Serial.println("V: Victory sound");
  Serial.println("E: Error sound");
  Serial.println("I: Imperial March");
  Serial.println("A: Siren (Alarm)");
  Serial.println("R: R2-D2 Beeps");
  Serial.println("D: R2-D2 Scream (Distress)");
  Serial.println("T: Tetris Theme");
  Serial.println("---------------------------------");
  
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    switch (input) {
      case 'b':
      case 'B':
        /** Play a simple beep at A4 (440Hz) for 200ms */
        Serial.println("Playing single beep");
        buzzer.beep(440, 200); // A4 for 200ms
        break;
        
      case 'm':
      case 'M':
        /** Play the Super Mario Bros theme */
        Serial.println("Playing Mario theme");
        buzzer.playMarioTheme();
        break;
        
      case 's':
      case 'S':
        /** Play an ascending startup sound */
        Serial.println("Playing startup sound");
        buzzer.playStartup();
        break;
        
      case 'v':
      case 'V':
        /** Play a victory fanfare */
        Serial.println("Playing victory sound");
        buzzer.playVictory();
        break;
        
      case 'e':
      case 'E':
        /** Play a descending error sound */
        Serial.println("Playing error sound");
        buzzer.playError();
        break;
        
      case 'i':
      case 'I':
        /** Play the Imperial March theme from Star Wars */
        Serial.println("Playing Imperial March");
        buzzer.playImperialMarch();
        break;
        
      case 'a':
      case 'A':
        /** Play a rising and falling siren sound */
        Serial.println("Playing Siren");
        buzzer.playSiren();
        break;
        
      case 'r':
      case 'R':
        /** Play random R2-D2 style beeps */
        Serial.println("Playing R2-D2 Beeps");
        buzzer.playR2D2();
        break;
        
      case 'd':
      case 'D':
        /** Play an R2-D2 distress scream */
        Serial.println("Playing R2-D2 Scream");
        buzzer.playR2D2Scream();
        break;
        
      case 't':
      case 'T':
        /** Play the Tetris theme melody */
        Serial.println("Playing Tetris Theme");
        buzzer.playTetris();
        break;
        
      default:
        // Ignore other inputs
        break;
    }
  }
  
  delay(500);
}