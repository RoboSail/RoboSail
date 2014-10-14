/*
  BoatCodeStarter0928
  Rev 9/28/14 D. Brancazio
  Reads receiver and wind sensor and creates commands to drive servo motors.
  Set to manual rudder and sail to function as a pass through for 
  commands from transmitter.
  Set to autonompus rudder and/or sail to try code for a basic sailing maneuver.
  (set manual rudder and sail flags to false)

  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  Connect servos to PWM servo ports 8 (rudder) and 9 (sailWinch)
  Wind Sensor is on digital pin 7
  Radio Control receiver is read on pins 11 (rudder), 12  (sail)
  
  Prints commands to serial monitor if asked (verbose ==true)
 Diane Brancazio and Eric Schneider
  www.robosail.org  --  diane@robosail.org

Sensors And Their Data Outputs

Wind Vane (direction wind is coming from) 
  US Digital MA3 Magnetic Encoder
  Data comes in 0-360°, set 0° = in irons, starboard wind is 90° and port wind is -90°
  Relative reading

Rudder
  clockwise = negative
  counter-clockwise = positive
  
Sail
  depends on the wind, but limit is 90 degrees
*/

//----------------LIBRARIES-------------------
#include <Wire.h>				   // for servos
#include <Servo.h>				 // for servos

/*------------FLAGS --------------------------------------*/
bool verbose = true;	     //Set to true to enable printlns to the Serial Monitor
bool manualSail = false;    //set to false for autonomous sail
bool manualRudder = true;  //set to false for autonomous rudder

/*-- Definitions and Global variables for Servos, Receiver, Compass, GPS, WInd Sensor ---------------*/

Servo rudderServo;      //create servo objects
Servo sailWinchServo; 
#define rudder      11  //input pins from receiver
#define sailwinch   12
#define windpin     7   //Wind sensor MA3 input pin



/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
  int rudderIn; // inputs from transmitter, in degrees
  int sailwinchIn;
  int rudderCMD = 0;
  int sailCMD = 45;
  int relWind = 0;    // The Relative Wind direction from wind sensor (degrees)
  int absvalRelWind;  // absolute value of wind direction

  float pi = 3.14159;



/************************** SETUP ***********************************/
void setup()  
{
  Serial.begin(9600);	// Set baud rate for the serial monitor
  Serial.println("BoatCode_XX0928 starting up, beginning intialization...");
  
/*--------- SetUp Receiver, Servos, Compass, GPS, Wind Sensor ------------------*/

  pinMode(rudderIn, INPUT);	  // Set RC receiver on digital input pins
  pinMode(sailwinchIn, INPUT);
  
  rudderServo.attach(8);
  sailWinchServo.attach(9);   // attaches the servo on pin 9 to the servo object 

  pinMode(windpin, INPUT);		// set Wind sensor pin as digital input

} // End of setup



/****************************** LOOP ***************************/
void loop()
{


  if (verbose)
  {
    Serial.print("\n\n\n\n\n\n\n"); // clear space for next run through loop
  }
 /*----------------------GET THE SENSOR VALUES------------------------*/
  relWind = getRelativeWind();      //read wind sensor

  readTransmitter();  // read RC transmitter via receiver and 
  			              // will put reading in rudderIn and sailwinchIn in degrees

 


/*--------------------------PROGRAM LOGIC ---------------------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

//This is the manual override code
//If the sail command lever is at 'full out', or 90deg then the boat is in auto mode
//If the sail command lever is anywhere else, then the controller has manual control
  if(sailwinchIn<80){
    //Manual Mode
    manualRudder = true;
    manualSail = true;
    if(verbose){
      Serial.println("Manual Override");
    }
  }
  else { 
    //Automatic Mode
    manualRudder = true; //For now, we are only doing automatic sail
    manualSail = false;
    if(verbose){
      Serial.println("Automatic Mode");
    }
  }


  if (manualRudder == true) 
  {
    rudderCMD = rudderIn;
  }
  else
  {
    // Sailing Code for rudder
    // assume maximum position of rudder is +-80.  
  
  }
  
  if (manualSail == true) 
  {
    sailCMD = sailwinchIn;
  }
  else
  {
    sailCMD = abs(relWind/2);
    
    // Sailing Code for sail
    // assume maximum position of sail is +-90.  
  }

  if (verbose)
  {
    Serial.print("     RudderCMD:   ");   Serial.print(rudderCMD); 
    Serial.print("  |        SailCMD:      ");  Serial.println(sailCMD);
  }
   
  //COMMAND THE MOTORS

  // will send input from transmitter or calculated postion 
  //depending on whether it is manual or autonomous
  commandMotor(rudderCMD, rudder);  
  commandMotor(sailCMD, sailwinch);

} // End of loop()

/*----------------- FUNCTION TO COMMAND A MOTOR------------------*/

void commandMotor(int pos, int servoname)
{
  if (servoname == rudder)
  {
    pos = map(pos, -80,80,40,140);
    rudderServo.write(pos); //servos go 0 to 180
  }
  if (servoname == sailwinch) 
  {
    pos = map(pos, 0,90,50,115);
    sailWinchServo.write(pos);
  }
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
  if (verbose)
  {
    Serial.print("Rudder Raw from TxRx: ");   
    Serial.print(rCH); Serial.print("  |  ");
  }

  if ((rCH < 1405) && (rCH > 1000)){  //noisy/out of range, don't change any values
    rCH = map(rCH, 1047,1405,-80,0);    // put reading in degrees, only go to 80 max
  }
   else if ((rCH < 2000) && (rCH >= 1405)){  //noisy/out of range, don't change any values
    rCH = map(rCH, 1405,1894,0,80);    // put reading in degrees, only go to 80 max
  }
  else
  {
    rCH = 0;
  }

  sCH = pulseIn(sailwinch, HIGH, 25000);
  if (verbose)
  {
    Serial.print("SailWinch Raw from TxRx:");  
    Serial.println(sCH);
  }
  if (!((sCH < 1000) || (sCH > 2000)))
  {
    sCH = map(sCH, 1140,1880,0,90);
  }
  else
  {
    sCH = 99;
  }
  if (verbose)
  {
    Serial.print("Rudder degrees from TxRx: ");   
    Serial.print(rCH); Serial.print("  |  ");
    Serial.print("SailWinch degrees from TxRx:");  
    Serial.println(sCH);
  }
  rudderIn = rCH;
  sailwinchIn = sCH;
  return;
}


/*----------------- FUNCTION TO READ WIND SENSORS--------------------*/

float getRelativeWind()	  //read MA3 PWM output
{
  unsigned long duration;   // used to read wind sensor as pulse length
  int WindDirection;        // degree value of wind from wind sensor
  duration = pulseIn(windpin, HIGH);
  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 1010, -180, 180);

  // constrain value between 0 and 360 to get rid of noisy outliers
  // use minus sign so values increase in CCW direction
  WindDirection = -min(max(WindDirection, -180), 180);
  
  if (verbose) {
    Serial.print("Relative wind direction:------------------------ ");
    Serial.println(WindDirection);
    Serial.println();
  }
  return WindDirection;
}
