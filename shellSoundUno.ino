/*
 * This sketch example demonstrates how the BMI160 on the
   Intel(R) Curie(TM) module can be used to read accelerometer data
   and translate it to an orientation
   
 * Now using Uno and Adafruit accelerometer, adapted 101 example code.
*/

#include "pitches.h"

// pins for accelerometer
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
 
// notes in the melody:
int melody[] = {
  NOTE_C3, NOTE_C3, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_A3, NOTE_G3, NOTE_F3, NOTE_F3, NOTE_E3, NOTE_E3, NOTE_D3, NOTE_D3, NOTE_C3};
 
// note durations: intialize all to quarter notes
int noteDurations[14];

int lastOrientation = - 1; // previous orientation (for comparison)

int noteToPlay = 0;

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");

  for (int i = 0; i < 14; i++){
   noteDurations[i] = 2;
  }

}

void loop() {
  int orientation = -1;   // the board's orientation
  String orientationString; // string for printing description of orientation
  /*
   * FROM ARDUINO 101
    The orientations of the board:
    0: flat, processor facing up
    1: flat, processor facing down
    2: landscape, analog pins down
    3: landscape, analog pins up
    4: portrait, USB connector up
    5: portrait, USB connector down
  */
  // read accelerometer:
  int x = ReadAxis(xInput);
  int y = ReadAxis(yInput);
  int z = ReadAxis(zInput);

  // calculate the absolute values, to determine the largest
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);

  if ( (absZ > absX) && (absZ > absY)) {
    // base orientation on Z
    if (z > 0) {
      orientationString = "up";
      orientation = 0;  
    } else {
      orientationString = "down";
      orientation = 1;
    }
  } else if ( (absY > absX) && (absY > absZ)) {
    // base orientation on Y
    if (y > 0) {
      orientationString = "digital pins up";
      orientation = 2;
    } else {
      orientationString = "analog pins up";
      orientation = 3;
    }
  } else {
    // base orientation on X
    if (x < 0) {
      orientationString = "connector up";
      orientation = 4;
    } else {
      orientationString = "connector down";
      orientation = 5;
    }
  }

  // send to p5 sketch
    Serial.println(orientation);

    // if not up orientation, play
    if(orientation != 0 && orientation != -1){
            int noteDuration = 1000/noteDurations[noteToPlay];
            tone(13, melody[noteToPlay], noteDuration);
    
            //pause for the note's duration plus 30 ms:
            delay(noteDuration +30);

            noteToPlay++;

            // start at beginning
            if(noteToPlay > 14){
              noteToPlay = 0;
            } 
    }

}




