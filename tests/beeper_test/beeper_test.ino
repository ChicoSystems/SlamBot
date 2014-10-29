#include "pitches.h"

/* Star Wars
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
  */
  
  /* ZELDAS LULLUBYE
  int melody[] = {  //zelda's lulluby//Define the melody as being the notes following using those defined in pitches.h
NOTE_E4, NOTE_G4, NOTE_D4, NOTE_C4, NOTE_D4, 
NOTE_E4, NOTE_G4, NOTE_D4,   0,     NOTE_E4, 
NOTE_G4, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_F4, 
NOTE_E4, NOTE_D4,    0,    NOTE_E4, NOTE_G4, 
NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, 
NOTE_D4,    0,    NOTE_E4, NOTE_G4, NOTE_D5, 
NOTE_C5, NOTE_G4};

int noteDurations[] = {                                               //Define the note durations, 1 to 1 with melody    1 = 8 beats 
  2,4,2,8,8,                                                          //                                                 2 = 4 beats (whole note)
  2,4,2,4,2,                                                          //                                                 4 = 2 beats (half note)
  4,2,4,2,8,                                                          //                                                 8 = 1 beats (quarter note)
  8,2,4,2,4,
  2,8,8,2,4,
  2,4,2,4,2,
  4,2 };
  */
  
  

 /* //Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0, 

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int noteDurations[] = {
  12, 12, 12, 12, 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12, 

  12, 12, 12, 12,
  12, 12, 12, 12, 
  12, 12, 12, 12, 
  12, 12, 12, 12, 

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};
*/

int melody[] = { NOTE_C4,0,NOTE_D4,0,NOTE_F4,0,
                 NOTE_D4,0,NOTE_F4,0,NOTE_G4,0,
                 NOTE_F4,0,NOTE_G4,0, NOTE_A4,0,
                 NOTE_G4,0,NOTE_A4,0, NOTE_B4,0,
                 NOTE_A4,0,NOTE_B4,0, NOTE_C5,0};
 
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8,8,4,
                        8,8,8,8,8,4,
                        8,8,8,8,8,4,
                        8,8,8,8,8,4,
                        8,8,8,8,8,4};

/* //Underworld melody
int melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4, 
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int noteDurations[] = {
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18,18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};
*/

void setup() {
 
}

void loop() {
  // no need to repeat the melody.
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < sizeof(noteDurations) / sizeof(int); thisNote++) {

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
  
  //delay(5000);
}
