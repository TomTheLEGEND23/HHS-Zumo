/**
 * @file Buzzer.cpp
 * @brief Implementation of the Buzzer class
 * @author Tom Steenvoorden
 * @date 10 May 2025
 */

#include "Buzzer.h"
#include <Arduino.h>

/**
 * Constructor initializes the buzzer
 */
Buzzer::Buzzer() {
    // Constructor - nothing special to initialize
}

/**
 * Destructor cleans up resources
 */
Buzzer::~Buzzer() {
    // Destructor - nothing to clean up
}

/**
 * Plays a single beep at the specified frequency and duration
 * @param frequency The frequency in Hz
 * @param duration The duration in milliseconds
 */
void Buzzer::beep(int frequency, int duration) {
    buzzer.playFrequency(frequency, duration, 15);
    delay(duration);
}

/**
 * Helper function to play a single note with a pause afterwards
 * @param frequency The frequency in Hz
 * @param duration The duration in milliseconds
 * @param pause The pause after the note in milliseconds
 */
void Buzzer::playNote(int frequency, int duration, int pause) {
    buzzer.playFrequency(frequency, duration, 15);
    delay(duration + pause);
}

/**
 * Plays the Super Mario Bros theme tune
 */
void Buzzer::playMarioTheme() {
    // Main Mario theme
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_C5, 150, 50);
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_G5, 150, 150);
    playNote(NOTE_G4, 150, 150);
    
    // Continue with more of the theme...
    playNote(NOTE_C5, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_G4, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_E4, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_A4, 150, 50);
    playNote(NOTE_B4, 150, 50);
    playNote(NOTE_AS4, 150, 50);
    playNote(NOTE_A4, 150, 50);
    
    playNote(NOTE_G4, 100, 25);
    playNote(NOTE_E5, 100, 25);
    playNote(NOTE_G5, 100, 25);
    playNote(NOTE_A5, 150, 50);
    playNote(NOTE_F5, 150, 50);
    playNote(NOTE_G5, 150, 50);
    playNote(NOTE_REST, 150, 50);
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_C5, 150, 50);
    playNote(NOTE_D5, 150, 50);
    playNote(NOTE_B4, 150, 150);
}

/**
 * Plays a startup sound - ascending notes
 */
void Buzzer::playStartup() {
    playNote(NOTE_C5, 100, 20);
    playNote(NOTE_E5, 100, 20);
    playNote(NOTE_G5, 100, 20);
    playNote(NOTE_C6, 200, 50);
}

/**
 * Plays a victory fanfare
 */
void Buzzer::playVictory() {
    playNote(NOTE_C5, 100, 20);
    playNote(NOTE_E5, 100, 20);
    playNote(NOTE_G5, 100, 20);
    playNote(NOTE_C6, 200, 100);
    playNote(NOTE_G5, 100, 20);
    playNote(NOTE_C6, 400, 100);
}

/**
 * Plays an error sound - descending notes
 */
void Buzzer::playError() {
    playNote(NOTE_G5, 100, 50);
    playNote(NOTE_E5, 100, 50);
    playNote(NOTE_C5, 300, 50);
}

/**
 * Plays the Imperial March theme from Star Wars
 */
void Buzzer::playImperialMarch() {
    // First measure
    playNote(NOTE_G4, 300, 50);
    playNote(NOTE_G4, 300, 50);
    playNote(NOTE_G4, 300, 50);
    playNote(NOTE_DS4, 250, 50);
    playNote(NOTE_AS4, 100, 50);
    
    // Second measure
    playNote(NOTE_G4, 300, 50);
    playNote(NOTE_DS4, 250, 50);
    playNote(NOTE_AS4, 100, 50);
    playNote(NOTE_G4, 500, 100);
}

/**
 * Plays a siren sound effect with rising and falling tones
 */
void Buzzer::playSiren() {
    for (int i = 0; i < 3; i++) {
        for (int freq = 440; freq < 880; freq += 20) {
            buzzer.playFrequency(freq, 10, 15);
            delay(10);
        }
        for (int freq = 880; freq > 440; freq -= 20) {
            buzzer.playFrequency(freq, 10, 15);
            delay(10);
        }
    }
}

/**
 * Plays random R2-D2 droid beeps
 */
void Buzzer::playR2D2() {
    // Random R2-D2 like beeps
    for (int i = 0; i < 10; i++) {
        int freq = random(500, 2500);
        int duration = random(30, 150);
        buzzer.playFrequency(freq, duration, 15);
        delay(duration + 20);
    }
}

/**
 * Plays an R2-D2 distress scream with descending tones
 */
void Buzzer::playR2D2Scream() {
    // R2-D2 panicked scream
    // Start with high frequency and rapidly descend
    for (int freq = 2500; freq > 500; freq -= 25) {
        buzzer.playFrequency(freq, 10, 15);
        delay(10);
    }
    
    // Brief pause
    delay(50);
    
    // Add some frantic modulation at the end
    for (int i = 0; i < 5; i++) {
        int baseFreq = 600 - (i * 50);
        buzzer.playFrequency(baseFreq + 100, 40, 15);
        delay(40);
        buzzer.playFrequency(baseFreq, 40, 15);
        delay(40);
    }
    
    // Final descending note
    for (int freq = 600; freq > 300; freq -= 15) {
        buzzer.playFrequency(freq, 8, 15);
        delay(8);
    }
}

/**
 * Plays the Tetris theme melody
 */
void Buzzer::playTetris() {
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_B4, 75, 25);
    playNote(NOTE_C5, 75, 25);
    playNote(NOTE_D5, 150, 50);
    playNote(NOTE_C5, 75, 25);
    playNote(NOTE_B4, 75, 25);
    playNote(NOTE_A4, 150, 50);
    playNote(NOTE_A4, 75, 25);
    playNote(NOTE_C5, 75, 25);
    playNote(NOTE_E5, 150, 50);
    playNote(NOTE_D5, 75, 25);
    playNote(NOTE_C5, 75, 25);
    playNote(NOTE_B4, 225, 75);
}