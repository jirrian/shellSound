/* 
 * Sketch for reading orientation of shell through an accelerometer. 
 * Based on orientation, plays music through VS1053 library and sends orientation to p5 via serial communication.
 * 
 * Now using Uno and ADXL335 accelerometer, adapted Arduino 101 accelerometer orientation example code.
 * using library for ADXL335 https://github.com/codedecaybr/ADXL335
 * 
 * Adapted code from player_simple example for Adafruit VS1053 Codec Breakout
*/

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

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);

boolean justPlayedTrackOne = false;

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");

  // for accelerometer library set up
  analogReference(EXTERNAL);
  acc.preset();

  // get initial accelerometer position
  initx = acc.getGX();
  inity = acc.getGY();
  initz = acc.getGZ();
  
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(5,5);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

}

void loop() {
  
  int orientation = -1;   // the board's orientation
  String orientationString; // string for printing description of orientation

  // read accelerometer:
  float x = acc.getGX();
  float y = acc.getGY();
  float z = acc.getGZ();

  // if x, y, z Gvalues are close to intial value, 
  // and are positive, then board is facing up
// && x < 0 && y < 0 && z < 0
  
  // board facing up
  if(abs(x - initx) <= 0.35 && abs(y - inity) <= 0.35 && abs(z - initz) <= 0.35){
    orientation = 0;
  }
  // board not facing up
  else{
    orientation = 1;
  }
//  Serial.print("x");
//  Serial.print(x);
//  Serial.print(", y");v               
//  Serial.print(y);
//  Serial.print(", z");
//  Serial.println(z);
  Serial.println(orientation);

    // if not up orientation, play music
    if(orientation != 0 && orientation != -1){
      // alternate tracks when user just picks up shell
      if(lastOrientation != orientation){
         if(justPlayedTrackOne == true){
            musicPlayer.startPlayingFile("track002.mp3");
            justPlayedTrackOne = false;
        }
        else{
          musicPlayer.startPlayingFile("track001.mp3");
          justPlayedTrackOne = true;
        }
      }
      // loop track if shell is still not in up orientation when track finishes (user is still listening)
      else{
        if(musicPlayer.stopped() && justPlayedTrackOne == true){
            musicPlayer.startPlayingFile("track001.mp3");
        }
        else if(musicPlayer.stopped() && justPlayedTrackOne == false){
          musicPlayer.startPlayingFile("track002.mp3");
        }
      }
    }
    else{
      musicPlayer.stopPlaying(); //stop player
    }

    // update orientation
    lastOrientation = orientation;

}




