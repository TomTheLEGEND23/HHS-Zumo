/**
 * @file Buzzer.h
 * @brief Header file for Zumo32U4 buzzer control
 * @author Tom Steenvoorden
 * @date 10 May 2025
 * 
 * This file provides a class for controlling the Zumo32U4 buzzer
 * and playing various sound effects and melodies.
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <Zumo32U4.h>

/** @defgroup NoteFrequencies Musical note frequencies in Hz
 *  @{
 */
#define NOTE_C4  262   /**< C4 note frequency (262 Hz) */
#define NOTE_CS4 277   /**< C#4/Db4 note frequency (277 Hz) */
#define NOTE_D4  294   /**< D4 note frequency (294 Hz) */
#define NOTE_DS4 311   /**< D#4/Eb4 note frequency (311 Hz) */
#define NOTE_E4  330   /**< E4 note frequency (330 Hz) */
#define NOTE_F4  349   /**< F4 note frequency (349 Hz) */
#define NOTE_FS4 370   /**< F#4/Gb4 note frequency (370 Hz) */
#define NOTE_G4  392   /**< G4 note frequency (392 Hz) */
#define NOTE_GS4 415   /**< G#4/Ab4 note frequency (415 Hz) */
#define NOTE_A4  440   /**< A4 note frequency (440 Hz) */
#define NOTE_AS4 466   /**< A#4/Bb4 note frequency (466 Hz) */
#define NOTE_B4  494   /**< B4 note frequency (494 Hz) */
#define NOTE_C5  523   /**< C5 note frequency (523 Hz) */
#define NOTE_CS5 554   /**< C#5/Db5 note frequency (554 Hz) */
#define NOTE_D5  587   /**< D5 note frequency (587 Hz) */
#define NOTE_DS5 622   /**< D#5/Eb5 note frequency (622 Hz) */
#define NOTE_E5  659   /**< E5 note frequency (659 Hz) */
#define NOTE_F5  698   /**< F5 note frequency (698 Hz) */
#define NOTE_FS5 740   /**< F#5/Gb5 note frequency (740 Hz) */
#define NOTE_G5  784   /**< G5 note frequency (784 Hz) */
#define NOTE_GS5 831   /**< G#5/Ab5 note frequency (831 Hz) */
#define NOTE_A5  880   /**< A5 note frequency (880 Hz) */
#define NOTE_AS5 932   /**< A#5/Bb5 note frequency (932 Hz) */
#define NOTE_B5  988   /**< B5 note frequency (988 Hz) */
#define NOTE_C6  1047  /**< C6 note frequency (1047 Hz) */
#define NOTE_REST 0    /**< Rest (no sound) */
/** @} */

/**
 * @class Buzzer
 * @brief A class for controlling the Zumo32U4 buzzer
 * 
 * This class provides methods to play various sounds and melodies
 * using the Zumo32U4 buzzer.
 */
class Buzzer {
public:
    /**
     * @brief Default constructor
     */
    Buzzer();
    
    /**
     * @brief Destructor
     */
    ~Buzzer();
    
    /**
     * @brief Play a single beep
     * @param frequency The frequency in Hz
     * @param duration The duration in milliseconds
     */
    void beep(int frequency, int duration);
    
    /**
     * @brief Play the Super Mario Bros theme
     */
    void playMarioTheme();
    
    /**
     * @brief Play a startup sound
     */
    void playStartup();
    
    /**
     * @brief Play a victory fanfare
     */
    void playVictory();
    
    /**
     * @brief Play an error sound
     */
    void playError();
    
    /**
     * @brief Play the Imperial March theme from Star Wars
     */
    void playImperialMarch();
    
    /**
     * @brief Play a rising and falling siren sound
     */
    void playSiren();
    
    /**
     * @brief Play R2-D2 style random beeps
     */
    void playR2D2();
    
    /**
     * @brief Play an R2-D2 distress scream
     */
    void playR2D2Scream();
    
    /**
     * @brief Play the Tetris theme melody
     */
    void playTetris();
    
private:
    /**
     * @brief The Zumo32U4Buzzer object
     */
    Zumo32U4Buzzer buzzer;
    
    /**
     * @brief Helper function to play a note with a pause
     * @param frequency The frequency in Hz (0 for rest)
     * @param duration The duration in milliseconds
     * @param pause The pause after the note in milliseconds
     */
    void playNote(int frequency, int duration, int pause);
};

#endif // BUZZER_H