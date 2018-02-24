#include "pitches.h"
 
#define VIN 9
// notes in the melody:

class Note {
public:
    Note(int, unsigned long);
    int pitch;
    unsigned long length;
};
Note::Note(int pitch, unsigned long length) {
    this->pitch=pitch;this->length=length;
}

int bpm = 120;
int beat_length = 1000/(bpm/60);

unsigned long lengths[] = {
    0,
    beat_length*4,
    beat_length*2,
    beat_length*4/3,
    beat_length,
    beat_length*4/5,
    beat_length*4/6,
    beat_length*4/7,
    beat_length/2
};
Note song[] = {
    Note(NOTE_B5, lengths[2]),
    Note(NOTE_C6, lengths[4]*3),
    Note(NOTE_B5, lengths[8]),
    Note(NOTE_C6, lengths[8]),
    Note(NOTE_B5, lengths[6]),
    Note(NOTE_A5, lengths[6]),
    Note(NOTE_C6, lengths[6]),
    Note(NOTE_B5, lengths[2]),
    Note(NOTE_E5, lengths[2]*3),
    Note(NOTE_B5, lengths[2]),
    Note(NOTE_C6, lengths[4]*3),
    Note(NOTE_B5, lengths[8]),
    Note(NOTE_C6, lengths[8]),
    Note(NOTE_B5, lengths[6]),
    Note(NOTE_A5, lengths[6]),
    Note(NOTE_C6, lengths[6]),
    Note(NOTE_B5, lengths[1]*2),

    Note(NOTE_AS5, lengths[2]),
    Note(NOTE_B5, lengths[4]*3),
    Note(NOTE_AS5, lengths[8]),
    Note(NOTE_B5, lengths[8]),
    Note(NOTE_AS5, lengths[6]),
    Note(NOTE_GS5, lengths[6]),
    Note(NOTE_B5, lengths[6]),
    Note(NOTE_AS5, lengths[2]),
    Note(NOTE_DS5, lengths[2]*3),
    Note(NOTE_AS5, lengths[2]),
    Note(NOTE_B5, lengths[4]*3),
    Note(NOTE_AS5, lengths[8]),
    Note(NOTE_B5, lengths[8]),
    Note(NOTE_AS5, lengths[6]),
    Note(NOTE_GS5, lengths[6]),
    Note(NOTE_B5, lengths[6]),
    Note(NOTE_AS5, lengths[1]*2),
};
 void setup() {
//    pinMode(VIN, OUTPUT)
//    digitalWrite(VIN, LOW);
    Serial.begin(9600);
}
 
void loop() {  
  for (int thisNote = 0; thisNote < (sizeof(song)/sizeof(*song)); thisNote++) {
    Serial.println(song[thisNote].pitch);
    Serial.println(song[thisNote].length);
    // pin8 output the voice, every scale is 0.5 sencond
    tone(8, song[thisNote].pitch, song[thisNote].length);
    // Output the voice after several minutes
    delay(song[thisNote].length);
  }
   
  // restart after two seconds 
  delay(2000);
}
