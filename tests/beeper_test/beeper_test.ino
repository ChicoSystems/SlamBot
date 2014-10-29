#include "pitches.h"

int melody[] = {
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G5, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_D4, NOTE_D4,
  NOTE_E4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_E4, NOTE_FS4, NOTE_D4, NOTE_D4,
  NOTE_E4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_D5, NOTE_A4, NOTE_A4, NOTE_D4, NOTE_D4,
  NOTE_E4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4*3, 4*3, 4*3,
  2, 2,
  4*3, 4*3, 4*3, 2, 4,
  4*3, 4*3, 4*3, 2, 4, 
  4*3, 4*3, 4*3, 2, (4*3)*2, 4*3,
  2, 2,
  4*3, 4*3, 4*3, 2, 4,
  4*3, 4*3, 4*3, 2, 4,
  4*3, 4*3, 4*3, 2, (4*3)*2, 4*3,
  8*3, 8, 8, 8, 8, 8,
  4*3, 4*3, 4*3, (4*3)*2, 4*3, 4, (4*3)*2, 4*3,
  8*3, 8, 8, 8, 8, 8,
  16*3, 16, 2, (4*3)*2, 4*3,
  8*3, 8, 8, 8, 8, 8};

void setup() {
 
}

void loop() {
  // no need to repeat the melody.
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 71; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(9, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(9);
  }
  delay(5000);
}
