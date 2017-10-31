/* 
 * Sketch for reading orientation of shell through an accelerometer. 
 * Based on orientation, plays music through VS1053 library and sends orientation to p5 via serial communication.
 * 
 * Now using Uno and ADXL335 accelerometer, adapted Arduino 101 accelerometer orientation example code.
 * using library for ADXL335 https://github.com/codedecaybr/ADXL335
 * 
 * Adapted code from player_simple example for Adafruit VS1053 Codec Breakout
*/

#include "pitches.h"
#include "ADXL335.h"

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// pins for accelerometer
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;

// initial positon of accelerometer
float initx, inity, initz;

// constructor for accelerometer library
ADXL335 acc(xInput, yInput, zInput);


int lastOrientation = - 1; // previous orientation (for comparison)

int noteToPlay = 0;

// for VS1053 breakout board
// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");

  // for accelerometer library set up
  analogReference(EXTERNAL);
  acc.preset();

  initx = acc.getGX();
  inity = acc.getGY();
  initz = acc.getGZ();
  
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
  if(abs(x - initx) <= 0.2 && abs(y - inity) <= 0.2 && abs(z - initz) <= 0.2 && x < 0 && y < 0 && z < 0){
    orientation = 0;
  }
  // board not facing up
  else{
    orientation = 1;
  }

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




