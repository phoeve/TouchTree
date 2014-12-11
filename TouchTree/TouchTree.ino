
#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus
Adafruit_MPR121 cap = Adafruit_MPR121();



uint8_t NoteTable[12] = {    // Table of notes to play
  0x10,                      // Ornament 0 - Hex Note
  0x11,                      // Ornament 1
  0x12,
  0x13,
  0x14,
  0x15,
  0x16,
  0x17,
  0x18,
  0x19,
  0x1A,
  0x1B                        // Ornament 11  
};   


// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


//#define DEBUG


void setup() {
  Serial.begin(31250);

  //Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    while (1);
  }

  cap.setThreshholds(0x41, 0x42);   // Default touch 0x41 and release 0x42
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
   

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
        // Play Touch Note
      noteOn(0x90, NoteTable[i], 0x45);  //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      delay(100);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
        // Play Release Note
      noteOn(0x90, 0x4A, 0x00);   //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
      delay(100);
    }
  }

  // reset our state
  lasttouched = currtouched;

}
