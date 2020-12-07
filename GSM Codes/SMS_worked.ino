#include <SoftwareSerial.h>
SoftwareSerial gsm(2,3);
String outMessage = "Hello world!";
String destinationNumber = "+91----------";

void setup()
{
  gsm.begin(4800); 
  gsm.print("AT+CMGF=1\r");
  delay(1000);
  gsm.println("AT + CMGS = \"" + destinationNumber +"\"");
  delay(1000);
  gsm.print(outMessage);
  delay(1000);
  gsm.write((char)26); //ctrl+z
  delay(1000);
  gsm.println("AT+CLTS=1");
  gsm.println("AT+CCLK?");
}


void loop()
{

}
