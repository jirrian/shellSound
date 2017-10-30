/*
 * This sketch example demonstrates how the BMI160 on the
   Intel(R) Curie(TM) module can be used to read accelerometer data
   and translate it to an orientation
   
 * Now using Uno and ADXL335 accelerometer, adapted 101 example code.
 * using library for ADXL335 https://github.com/codedecaybr/ADXL335
*/

#include "pitches.h"
#include "ADXL335.h"

// pins for accelerometer
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;

// constructor for accelerometer library
ADXL335 acc(xInput, yInput, zInput);
 
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

  // for accelerometer library set up
  analogReference(EXTERNAL);
  acc.preset();
  
  for (int i = 0; i < 14; i++){
   noteDurations[i] = 2;
  }

}

void loop() {
  
  int orientation = -1;   // the board's orientation
  String orientationString; // string for printing description of orientation

  // read accelerometer:
  float x = acc.getGX();
  float y = acc.getGY();
  float z = acc.getGZ();

  // if x, y, z Gvalues are close to 0 (absolute values less than 0.2), 
  // and are positive, then board is facing up
  
  // board facing up
  if(x <= 0.2 && y <= 0.2 && z <= 0.2 && x > 0 && y > 0 && z > 0){
    orientation = 0;
  }
  // board not facing up
  else{
    orientation = 1;
  }

  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.print(",");
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




