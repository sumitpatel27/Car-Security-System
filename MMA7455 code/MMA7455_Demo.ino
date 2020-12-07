

#include <Wire.h>               //Include the Wire library
#include <MMA_7455.h>           //Include the MMA_7455 library

MMA_7455 accel= MMA_7455();    // Make MMA7455 object

char xVal, yVal, zVal;          // Return value variables

void setup() {
  Serial.begin(9600);           // Use the Serial Monitor window at 9600 baud
  
  // Set the g force sensitivity: 2=2g, 4=4g, 8-8g
  accel.initSensitivity(2);
  
 // Provide oiffset values so that sensor displays 0, 0, 63
 //  (or close to it) when positioned on a flat surface, all pins
 //  facing down
 
 // Update the numbers with your own values from the MMA7455_CalibrateOffset sketch.
  accel.calibrateOffset(0, 0, 0);
}

void loop() {
 
  // Get the X, Y, anx Z axis values from the device
  xVal = accel.readAxis('x');   // Read X Axis
  yVal = accel.readAxis('y');   // Read Y Axis
  zVal = accel.readAxis('z');   // Read Z Axis
  
  // Display them in the Serial Monitor window.
  Serial.print("X: = ");
  Serial.print(xVal, DEC);
  Serial.print("   Y: = ");
  Serial.print(yVal, DEC);
  Serial.print("   Z: = ");
  Serial.println(zVal, DEC);
  delay(1000);
}
