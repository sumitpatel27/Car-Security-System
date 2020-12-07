
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>               //Include the Wire library
#include <MMA_7455.h>           //Include the MMA_7455 library
#include <LiquidCrystal_I2C.h> // scl to a5 , sda to a4 
LiquidCrystal_I2C lcd(0x3f,16,2);  // Set the LCD I2C address
SoftwareSerial GPS_SoftSerial(11, 10);/* (Rx, Tx) */
MMA_7455 accel= MMA_7455();    // Make MMA7455 object their SCL AND SDA IS CONNECT TO ARDUINO's 2nd SCL AND SDA above ARAF pin
TinyGPSPlus gps; //gps to 10 and 100
SoftwareSerial gsm(2,3);//rx to 3 , tx to 2 and digital GND
String destinationNumber = "+917016502514";/////set number
String destinationNumber1 = "+917016502514";  
char xVal1, yVal1, zVal1;          // Return value variables
char s;
int vibr_pin=4; //vcc in 3.3v and vibration pin to 4 

void setup() {
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("GETTING DATA....");
  delay(10000);
  lcd.setCursor(0,1);
  // Set the g force sensitivity: 2=2g, 4=4g, 8-8g
  gsm.begin(4800);
  gsm.println("ATD+917016502514;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  lcd.clear();
  lcd.print("Connecting..."); // print response over serial port
  delay(5000);
  Serial.begin(9600);           // Use the Serial Monitor window at 9600 baud
  GPS_SoftSerial.begin(9600);
  accel.initSensitivity(2);
  
 // Provide oiffset values so that sensor displays 0, 0, 63
 //  (or close to it) when positioned on a flat surface, all pins
 //  facing down
 
 // Update the numbers with your own values from the MMA7455_CalibrateOffset sketch.
  accel.calibrateOffset(0, 0, 0);
  lcd.clear();
  lcd.print("SYSTEM IS");
  lcd.setCursor(0,1);
  lcd.print("CONNECTED");
}

void loop() {
 smartDelay(1000); /* Generate precise delay of 1ms */
        int year_val,xVal,yVal,zVal,ind_hr_val,ind_min_val,ind1_min_val;
        unsigned long start;
        double lat_val, lng_val,speed_val, alt_m_val;
        uint8_t hr_val, min_val,day_val,month_val, sec_val;
        bool loc_valid, alt_valid, time_valid ,speed1_val ,date1_val;
  // Display them in the Serial Monitor window.
  lcd.clear();
lat_val = gps.location.lat(); /* Get latitude data */
lng_val = gps.location.lng(); /* Get longtitude data */
loc_valid = gps.location.isValid(); /* Check if valid location data is available */
       if (!loc_valid)
        { 
          lcd.clear();
          lcd.setCursor(0,0);         
          lcd.print("LAT=");
          lcd.print("*****");
          lcd.setCursor(0,1);
          lcd.print("LNG=");
          lcd.print("*****");
          delay(500);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0,0);         
          lcd.print("LAT=");
          lcd.print(lat_val, 6);
          lcd.setCursor(0,1);
          lcd.print("LNG=");
          lcd.print(lng_val, 6);
          delay(500);
        }
 hr_val = gps.time.hour(); /* Get hour */
 min_val = gps.time.minute();  /* Get minutes */
 sec_val = gps.time.second();  /* Get seconds */
time_valid = gps.time.isValid();  /* Check if valid time data is available */
ind1_min_val= (min_val+30);
if (ind1_min_val<60)
        {
          ind_hr_val=hr_val+5;
          ind_min_val=ind1_min_val;
        }
        else
        {
          ind_hr_val=hr_val+6;
          ind_min_val=ind1_min_val-60;
        }
        if (!time_valid)
        {
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("Time : ");
          lcd.setCursor(0,1);
          lcd.print("*****");
          delay(500);
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "%02d:%02d:%02d \n", ind_hr_val, ind_min_val, sec_val);
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("TIME : (IND)");      
          lcd.setCursor(0,1);
          lcd.print(time_string);     
          lcd.setCursor(9,1);
          lcd.print("       ");   
          delay(500); 
        }
speed_val = gps.speed.kmph(); /*get the speed data*/
s = speed_val; /*speed value set as s*/
speed1_val = gps.speed.isValid();/*check the data*/
        if (!speed1_val)
        {
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("SPEED :");
          lcd.setCursor(0,1);
          lcd.print("*****");
          delay(500);
        }
        else 
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("SPEED :  KM/H"); 
          lcd.setCursor(0,1);
          lcd.print(speed_val);         
          delay(500);   
           if (s>50) {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("speed lmt exceed");
            sendmsg2();
            lcd.setCursor(0,1);
            lcd.print("MSG SENT");
            delay(500);
            }  
        }
day_val = gps.date.day(); /*Get date Value*/
month_val = gps.date.month(); /*get the month value*/
year_val = gps.date.year(); /*get the year value*/
date1_val = gps.date.isValid(); /*check data is valid*/
         if (!date1_val)
        {
          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("DATE :");
          lcd.setCursor(0,1);
          lcd.print("*****");
          delay(500);
        }
        else 
        {

          lcd.clear();
          lcd.setCursor(0,0); 
          lcd.print("DATE :");
          lcd.setCursor(0,1);
          lcd.print(day_val);   
          lcd.print("/");
          lcd.print(month_val);   
          lcd.print("/");
          lcd.print(year_val);   
          delay(500);   
        }
int val;
val=digitalRead(vibr_pin);
         if (val==1)   {
                  lcd.clear();
                  lcd.setCursor(1,0);
                  lcd.print("VIB STATUS");
                  lcd.setCursor(1,1);
                  lcd.print("ACTIVATED");
                  delay(500);
                  sendmsg1();
                 }
          else {
                  lcd.clear();
                  lcd.setCursor(1,0);
                  lcd.print("VIB STATUS");
                  lcd.setCursor(1,1);
                  lcd.print("NORMAL");
                  delay(500);
                }
  // Get the X, Y, anx Z axis values from the device
  xVal1 = accel.readAxis('x');   // Read X Axis
  yVal1 = accel.readAxis('y');   // Read Y Axis
  zVal1 = accel.readAxis('z');   // Read Z Axis
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  X    Y    Z ");
        lcd.setCursor(0,1);
  lcd.print(xVal1,DEC);
  lcd.setCursor(6,1);
  lcd.print(yVal1,DEC);
  lcd.setCursor(12,1);
  lcd.print(zVal1,DEC);
  delay(50);
       
  if (xVal1 < -63){ sendmsg();}
  else if (xVal1 >  45){ sendmsg();}
  else if (yVal1 < -75){ sendmsg();}
  else if (yVal1 >  35){ sendmsg();}   
  else if (zVal1 < 0) {sendmsg();}
}



static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

////////////////////sendsms//////////////
void sendmsg()
{gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber +"\"");
  delay(1000);
  gsm.print(gps.time.hour());
  gsm.print(":");
  gsm.print(gps.time.minute());
  gsm.print(":");
  gsm.print(gps.time.second());
  gsm.print("--");
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
/////////////////////////////////////////
////////////////////////sendsms1//////////
void sendmsg1()
{gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber1 +"\"");
  delay(1000);
    gsm.print(gps.time.hour());
  gsm.print(":");
  gsm.print(gps.time.minute());
  gsm.print(":");
  gsm.print(gps.time.second());
  gsm.print("--");
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
//////////////////////////////////////////
////////////////////////sendsms1//////////
void sendmsg2()
{gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber1 +"\"");
  delay(1000);
    gsm.print(gps.time.hour());
  gsm.print(":");
  gsm.print(gps.time.minute());
  gsm.print(":");
  gsm.print(gps.time.second());
  gsm.print("--");
   gsm.print("Your Car Speed is");
  gsm.print(gps.speed.kmph());
  gsm.print("  SPEED LIMIT EXCEED at :- www.google.com/maps/?q=");
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
//////////////////////////////////////////


