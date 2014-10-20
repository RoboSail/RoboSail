/*

 2014-10-19 Boat code for the Pink team (D).
 
 This code will allow manual rudder control, and when commanded (via sail channel
 on the radio) will begin an autonomous sequence. This sequence will maintain a
 tack for some number of seconds, then move on to the next tack. In this way a
 path following algorithm can be built.
 
 As it stands now, the path specificed roughly sails a Mercury yellow test as
 given at CBI.
 
 ---------------------------------------------------------------------------------

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
 
 Prints commands to serial monitor if asked (verbose
 
 ==true)
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
unsigned long tackDuration [] = {
  30*1000, //0
  60*1000, //1
  60*1000, //2
  45*1000, //3
  20*1000, //4
  24*1000, //5
  24*1000, //6
  999*1000 //7
};
int headings [] = {
  -45,
  45,
  -45,
  45,
  135,
  -135,
  135,
  -135,
  135
};
int currentIndex =-1;
unsigned long nextActionTime = -1;

int desiredHeading = -1;

/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
int rudderIn; // inputs from transmitter, in degrees
int sailwinchIn;
int rudderCMD = 0;
int sailCMD = 45;
int relWind = 0;    // The Relative Wind direction from wind sensor (degrees)
//int absvalRelWind;  // absolute value of wind direction

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

  if (sailwinchIn < 1400){
    //keep relwind 60
    if (millis() > nextActionTime) {
      currentIndex++;
      nextActionTime += tackDuration[currentIndex];
      desiredHeading = headings[currentIndex];
    }
    int error = calculateDistance (relWind, desiredHeading);
    rudderCMD = error;
    if (rudderCMD > 80){
      rudderCMD = 80;
    }
    if (rudderCMD < -80){
      rudderCMD = -80;
    }


    // Sailing Code for rudder
    // assume maximum position of rudder is +-80.  

  }
  else
  {
    currentIndex =-1;
    nextActionTime =millis();
    rudderCMD = rudderIn;
  }

  if (manualSail == true) 
  {
    sailCMD = sailwinchIn;
  }
  else
  {
    if(abs(relWind)<45){
      sailCMD = -35;
      //irons
    }
    else if (abs(relWind)>=45 && abs(relWind)<100){
      sailCMD = map(abs(relWind), 45, 120, -35, 30);
      //Haul
    }
    else{
      sailCMD = 30;
      //run
    }
    // Sailing Code for sail
    // assume maximum position of sail is +-90.  

  }

  if (verbose)
  {
    Serial.print("     RudderCMD:   ");   
    Serial.print(rudderCMD); 
    Serial.print("  |        SailCMD:      ");  
    Serial.println(sailCMD);
  }

  //COMMAND THE MOTORS

  // will send input from transmitter or calculated postion 
  //depending on whether it is manual or autonomous
  commandMotor(rudderCMD, rudder);  
  commandMotor(sailCMD, sailwinch);

} // End of loop()

/*----------------- FUNCTION TO COMMAND A MOTOR------------------*/

int calculateDistance(int start, int finish){
  int A=finish - start;
  int B;
  if (A>0){
    B=A - 360;
  }
  else{
    B=A + 360;
  }
  if (abs(A) < abs(B)){
    return A;
  }
  else{
    return B;
  }

}

void commandMotor(int pos, int servoname)
{
  if (servoname == rudder)
  {
    rudderServo.write(pos+90); //servos go 0 to 180
  }
  if (servoname == sailwinch) 
  {
    sailWinchServo.write(pos+90);
  }
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
  if (!((rCH < 1000) || (rCH > 2000))){  //noisy/out of range, don't change any values
    rCH = map(rCH, 1069,1875,-80,80);    // put reading in degrees, only go to 80 max
  }
  else
  {
    rCH = 99;
  }

  sCH = pulseIn(sailwinch, HIGH, 25000);
  //if (!((sCH < 1000) || (sCH > 2000)))
  //{
  //  sCH = map(sCH, 1000,2000,0,90);
  //}
  //else
  //{
  //  sCH = 99;
  //}
  if (verbose)
  {
    Serial.print("Rudder from TxRx: ");   
    Serial.print(rCH); 
    Serial.print("  |  ");
    Serial.print("SailWinch from TxRx:");  
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
