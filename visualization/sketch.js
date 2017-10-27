var serial;          // variable to hold an instance of the serialport library
var portName = '/dev/cu.usbmodem1411';  // fill in your serial port name here

var x,y; // accelerometer values from arduino
var orientation, lastOrientation; // orientation from arduino (int)

//var absX, absY; // absolute value of x and y

var oceanVideo; // video

//var canvas;

function setup() {
  // canvas = createCanvas(600,400);
  // canvas.parent("canvas");
  // background(0,0,0,0);

  // initialize orientation
  orientation = 0;
  lastOrientation = -1;

  // get element for video
  oceanVideo = select('#video'); 

  //oceanVideo = createVideo("oceanWavesCut.mov");
  
  serial = new p5.SerialPort();       // make a new instance of the serialport library
  serial.on('connected', serverConnected); // callback for connecting to the server
  serial.on('open', portOpen);        // callback for the port opening
  serial.on('data', serialEvent);     // callback for when new data arrives
  serial.on('error', serialError);    // callback for errors
  serial.on('close', portClose);      // callback for the port closing
 
  serial.list();                      // list the serial ports
  serial.open(portName);              // open a serial port
}

function serverConnected() {
  print('connected to server.');
}
 
function portOpen() {
  print('the serial port opened.')
}
 
function serialEvent() {
  // read a string from the serial port
  // until you get carriage return and newline:
  var inString = serial.readStringUntil('\r\n');
 
  //check to see that there's actually a string there:
  if (inString.length > 0 ) {
    // var sensors = split(inString, ',');            // split the string on the commas
    // if (sensors.length > 1) {                      // if there are three elements
    //   x = sensors[0];   
    //   y = sensors[1]; 

    // }

    orientation = int(inString);
  }
}
 
function serialError(err) {
  print('Something went wrong with the serial port. ' + err);
}
 
function portClose() {
  print('The serial port closed.');
}

function draw() {
     print(orientation);
  // if(orientation != 0){
  //   oceanVideo.loop();
  // }
  // else{
  //   oceanVideo.pause();
  // }

  // fade to black based on accelerometer values mapped between 0 and 255
  // if(x > y){
  //   fill(0,0,0,map(x, -1, 1, 0, 255));
  //   rect(0,0,560,315);
  //   print("x");
  // }
  // else{
  //   fill(0,0,0,map(y, -1, 1, 0, 255));
  //   rect(0,0,560,315);
  //   print("y");
  // }
  
  // hard cut to black screen based on accelerometer vales
  // if(float(abs(x)) < 0.80){
  //   fill(0);
  //   rect(0,0,560,315);
  // }
  // else{
  //   clear();
  // }

  // check if orientation changed
  // is up wasnt up
  if (orientation == 0 && lastOrientation != 0) {
    oceanVideo.hide();
  }
  // isnt up was up
  else if(orientation != 0 && lastOrientation == 0){
    oceanVideo.show();
  }
  lastOrientation = orientation;
}


