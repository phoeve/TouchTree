
#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus
Adafruit_MPR121 cap = Adafruit_MPR121();



uint8_t NoteTable[12] = {    // Table of notes to play
  0x1E,                      // Ornament 0 - Hex Note
  0x1E,                      // Ornament 1
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E                        // Ornament 11  
};   


// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;


//#define DEBUG


void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");
#else
  Serial.begin(31250);
#endif

  //Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
#ifdef DEBUG
    Serial.println("MPR121 not found, check wiring?");
#endif
    while (1);
  }

#ifdef DEBUG
  Serial.println("MPR121 found!");
#endif
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
#ifdef DEBUG
      Serial.print(i); Serial.println(" touched");
#else
        // Play Touch Note
      noteOn(0x90, NoteTable[i], 0x45);  //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
      delay(100);
#endif
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
#ifdef DEBUG
      Serial.print(i); Serial.println(" released");
#else
        // Play Release Note
      noteOn(0x90, 0x4A, 0x00);   //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
      delay(100);
#endif
    }
  }

  // reset our state
  lasttouched = currtouched;

}