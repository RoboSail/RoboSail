/* GPSTest rev 4/22/2016
© 2014-2016 RoboSail
Find detailed description in Decription tab */

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <UsefulCalcs.h>
#include "GPS.h"
Adafruit_GPS GPS(&Serial1);


void setup()
{
initialize();
}

void loop() // run over and over again
{
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived())
  {
    char* LastNMEA; // pointer to GPS data
    LastNMEA = GPS.lastNMEA(); // read the string and set the newNMEAreceived() flag to false
    if (!GPS.parse(LastNMEA)) {return; } // failed to parse a sentence; just wait for another

    if (print_raw_gps) {Serial.println(LastNMEA);}
    if (print_parsed_gps) {PrintGPSInfo();}
    
    if (GPS.fix)
    {
      getposition();
    }
  }
  
  // wait 2 seconds before repeating loop
  delay(2000);
  
}


