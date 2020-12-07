#include "TinyGPS++.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MMA_7455.h> //Include the MMA_7455 library
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,16,2);  // Set the LCD I2C address

SoftwareSerial gsm(2,3);
static const int RXPin = 11, TXPin = 10;
static const uint32_t GPSBaud = 9600;
String destinationNumber = "+918460502706";
String destinationNumber1 = "+918460502706";
// The TinyGPS++ object
TinyGPSPlus gps;
MMA_7455 mySensor = MMA_7455(); //Make an instance of MMA_7455

/*-----( Declare Variables )-----*/
char xVal, yVal, zVal; //Variables for the values from the sensor

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
int vibr_pin=4;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  Serial.begin(115200);
  ss.begin(GPSBaud);
    
  lcd.print("GPS Location"); 
  delay(1000);
  lcd.clear(); 
  
   // Set the sensitivity you want to use
  // 2 = 2g, 4 = 4g, 8 = 8g
  mySensor.initSensitivity(2); // Good for "Tilt" measurements
 // Calibrate the Offset. Calibrate, in Flat position, try to
//    get: xVal = 0, yVal = 0, zVal = +63 (1 G)
}
void loop()
{
  xVal = mySensor.readAxis('x'); //Read out the 'x' Axis
  yVal = mySensor.readAxis('y'); //Read out the 'y' Axis
  zVal = mySensor.readAxis('z'); //Read out the 'z' Axis
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    lcd.print(F("No GPS detected"));
    while(true);
  }
  //set start
  int val;
  val=digitalRead(vibr_pin);
  if(val==1)
  {
     lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("VIB STATUS");
    lcd.setCursor(1,1);
    lcd.print("ACTIVATED");
    delay(500);
    sendmsg1();
   }
   else
   {
    displayInfo();
    }
  //set end
  ///////////////tilt sensor///////////////
  if (xVal < -40) sendmsg();
  if (xVal >  30) sendmsg();
  if (yVal < -55) sendmsg();
  if (yVal >  20 ) sendmsg();   
  if (zVal < -33 ) sendmsg(); 
  /////////////////////////////////////////
}


////////////////////display////////////////
void displayInfo()
{   
  if (gps.location.isValid())
  {
    lcd.setCursor(0,0);
    lcd.print("Lat=");
    lcd.print(gps.location.lat()   , 6);
    lcd.setCursor(0,1);
    lcd.print("Lng=");
    lcd.print(gps.location.lng()   , 6);
    delay(1000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("VIB STATUS");   
    lcd.setCursor(1,1); 
    lcd.print("NORMAL");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("X   Y   Z");
    lcd.setCursor(1,1);
    lcd.print(xVal, DEC);
    lcd.setCursor(4,1);
    lcd.print(yVal, DEC);
    lcd.setCursor(8,1);
    lcd.println(zVal, DEC);
    delay(1000);
  }
  else
  {
   lcd.setCursor(0,0);
   lcd.print("Finding");
   lcd.setCursor(0,1);
   lcd.print("Location");
   delay(200);
  lcd.clear();
  }  
}
///////////////////////////////

/////////////sms////////////////
void sendmsg()
{gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber +"\"");
  delay(1000);
  gsm.print("Accident of car -GJ6 FC 7259- at :- www.google.com/maps/?q=");
  gsm.print(gps.location.lat()    , 6);
  gsm.print(",");
   gsm.print(gps.location.lng()    , 6);
  delay(1000);
  gsm.write((char)26); //ctrl+z
  delay(1000);
  gsm.println("AT+CLTS=1");
  gsm.println("AT+CCLK?");
  delay(2000);//
  lcd.clear();
  lcd.print("LOCATION SENT");
  delay(1000);
}
void sendmsg1()
{gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber1 +"\"");
  delay(1000);
  gsm.print("CAR ENGINE START at :- www.google.com/maps/?q=");
  gsm.print(gps.location.lat()    , 6);
  gsm.print(",");
   gsm.print(gps.location.lng()    , 6);
  delay(1000);
  gsm.write((char)26); //ctrl+z
  delay(1000);
  gsm.println("AT+CLTS=1");
  gsm.println("AT+CCLK?");
  delay(2000);//
  lcd.clear();
  lcd.print("LOCATION SENT");
  delay(1000);
}
/////////////////////////////////
