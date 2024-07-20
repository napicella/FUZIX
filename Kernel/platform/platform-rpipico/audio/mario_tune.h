#ifndef mario_tune_h
#define mario_tune_h

#include <stdint.h>

// Mario tune. Taken from:
// https://codebender.cc/sketch:361198#Circuit%20Playground%20Mario%20Theme%20Player.ino

// intro notes and durations
const int smbIntroNotes[] = {NOTE_E4, NOTE_E4, 0, NOTE_E4, 0,       NOTE_C4, NOTE_E4,
                       0,       NOTE_G4, 0, 0,       NOTE_G3, 0,       0};
const int smbIntroDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 4};

// part A notes and durations
const int smbANotes[] = {
    NOTE_C4, 0,        0,       NOTE_G3, 0,        NOTE_E3, 0,       0,
    NOTE_A3, 0,        NOTE_B3, 0,       NOTE_AS3, NOTE_A3, 0,       NOTE_G3,
    NOTE_E4, NOTE_G4,  NOTE_A4, 0,       NOTE_F4,  NOTE_G4, 0,       NOTE_E4,
    0,       NOTE_C4,  NOTE_D4, NOTE_B3, 0,        NOTE_C4, 0,       0,
    NOTE_G3, 0,        NOTE_E3, 0,       0,        NOTE_A3, 0,       NOTE_B3,
    0,       NOTE_AS3, NOTE_A3, 0,       NOTE_G3,  NOTE_E4, NOTE_G4, NOTE_A4,
    0,       NOTE_F4,  NOTE_G4, 0,       NOTE_E4,  0,       NOTE_C4, NOTE_D4,
    NOTE_B3, 0};
const int smbADurations[] = {8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       6, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8,
                       8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6,
                       6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4};

// part B notes and durations
const int smbBNotes[] = {
    0,        NOTE_G4,  NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,        NOTE_E4,
    0,        NOTE_GS3, NOTE_A3,  NOTE_C4,  0,        NOTE_A3,  NOTE_C4,
    NOTE_D4,  0,        NOTE_G4,  NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,
    NOTE_E4,  0,        NOTE_C5,  0,        NOTE_C5,  NOTE_C5,  0,
    0,        0,        NOTE_G4,  NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,
    NOTE_E4,  0,        NOTE_GS3, NOTE_A3,  NOTE_C4,  0,        NOTE_A3,
    NOTE_C4,  NOTE_D4,  0,        NOTE_DS4, 0,        0,        NOTE_D4,
    0,        NOTE_C4,  0,        0,        0,        0,        NOTE_G4,
    NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,        NOTE_E4,  0,        NOTE_GS3,
    NOTE_A3,  NOTE_C4,  0,        NOTE_A3,  NOTE_C4,  NOTE_D4,  0,
    NOTE_G4,  NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,        NOTE_E4,  0,
    NOTE_C5,  0,        NOTE_C5,  NOTE_C5,  0,        0,        0,
    NOTE_G4,  NOTE_FS4, NOTE_FF4, NOTE_DS4, 0,        NOTE_E4,  0,
    NOTE_GS3, NOTE_A3,  NOTE_C4,  0,        NOTE_A3,  NOTE_C4,  NOTE_D4,
    0,        NOTE_DS4, 0,        0,        NOTE_D4,  0,        NOTE_C4,
    0,        0,        0};
const int smbBDurations[] = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8,
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8, 8, 8,
                       8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 4, 2,
                       4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8,
                       8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8, 8, 8,
                       8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 4, 2};

// part C notes and durations
const int smbCNotes[] = {
    NOTE_C4, NOTE_C4, 0,       NOTE_C4, 0,       NOTE_C4, NOTE_D4, 0,
    NOTE_E4, NOTE_C4, 0,       NOTE_A3, NOTE_G3, 0,       0,       NOTE_C4,
    NOTE_C4, 0,       NOTE_C4, 0,       NOTE_C4, NOTE_D4, NOTE_E4, 0,
    NOTE_C4, NOTE_C4, 0,       NOTE_C4, 0,       NOTE_C4, NOTE_D4, 0,
    NOTE_E4, NOTE_C4, 0,       NOTE_A3, NOTE_G3, 0,       0,       NOTE_E4,
    NOTE_E4, 0,       NOTE_E4, 0,       NOTE_C4, NOTE_E4, 0,       NOTE_G4,
    0,       0,       NOTE_G3, 0,       0};
const int smbCDurations[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8,
                       8, 8, 8, 8, 8, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                       8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 4};

// part D notes and durations
const int smbDNotes[] = {
    NOTE_E4,  NOTE_C4, 0,       NOTE_G3, 0,       NOTE_GS3, 0,       NOTE_A3,
    NOTE_F4,  0,       NOTE_F4, NOTE_A3, 0,       0,        NOTE_B3, NOTE_A4,
    NOTE_A4,  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4,  0,       NOTE_A3,
    NOTE_G3,  0,       0,       NOTE_E4, NOTE_C4, 0,        NOTE_G3, 0,
    NOTE_GS3, 0,       NOTE_A3, NOTE_F4, 0,       NOTE_F4,  NOTE_A3, 0,
    0,        NOTE_B3, NOTE_F4, 0,       NOTE_F4, NOTE_F4,  NOTE_E4, NOTE_D4,
    NOTE_C4,  NOTE_E3, 0,       NOTE_E3, NOTE_C2, 0,        0};
const int smbDDurations[] = {8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 4, 6, 6, 6, 6, 6,
                       6, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8,
                       8, 8, 4, 8, 8, 8, 8, 6, 6, 6, 8, 8, 8, 8, 8, 8, 4};

#endif