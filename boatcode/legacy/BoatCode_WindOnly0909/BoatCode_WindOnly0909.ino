/*
  Reads receiver and wind sensor and attempts to command boat to sail Close haul and tack
 rev 9/9/14
  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  Connect servos to PWM servo ports 8 (rudder) and 9 (sailWinch)
  Wind Sensor i on digital pin 7
  Radio Control receiver is read on pins 11 (rudder), 12  (sail)
  prints commands to serial monitor if asked (verbose ==true)
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
#include <Wire.h>				// for servos
#include <Servo.h>				// for servos

/*------------FLAGS --------------------------------------*/
bool verbose = true;	//Set to true to enable printlns to the Serial Monitor
bool manualSail = true; //set to false for autonomous sail
bool manualRudder = true;  //set to false for autonomous rudder

/*-- Definitions and Global variables for Servos, Receiver, Compass, GPS, WInd Sensor ---------------*/

Servo rudderServo;  //create servo objects
Servo sailWinchServo; 
#define rudder      11  //input pins from receiver
#define sailwinch   12
#define windpin       7   	//Wind sensor MA3 input pin
unsigned long duration;		// used to read wind sensor
int WindDirection;


/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
  // Things we can get from the sensors
  int relWind = 0;              // From wind sensor (degrees)
  int absvalRelWind;  // absolute value of wind sensor

  // Useful variables

  int rudderIn; // inputs from transmitter, in degrees
  int sailwinchIn;
  int rudderCMD = 0;
  int sailCMD = 45;

  bool RC_good = false;			//not currently used
  bool wind_good = false;		//not currently used

  #define ironsRange    45
  float pi = 3.14159;
// Initialize the calculator object


/************************** SETUP ***********************************/
void setup()  
{
  Serial.begin(9600);	// Set baud rate for the serial monitor
  Serial.println("BoatCode_WindOnly0909 starting up, beginning intialization...");
  
/*--------- SetUp Receiver, Servos, Compass, GPS, Wind Sensor ------------------*/

  pinMode(rudderIn, INPUT);	  // Set RC receiver on digital input pins
  pinMode(sailwinchIn, INPUT);
  RC_good = true;	//need to test for this?
  
  rudderServo.attach(8);
  sailWinchServo.attach(9);   // attaches the servo on pin 9 to the servo object 

  pinMode(windpin, INPUT);		// set Wind sensor pin as digital input
  wind_good = true;

} // End of setup



/****************************** LOOP ***************************/
void loop()
{


  if (verbose) Serial.print("\n\n\n\n\n\n\n");	// clear space for next run through loop
 /*----------------------GET THE SENSOR VALUES------------------------*/
  relWind = getRelativeWind();      //read wind sensor

  readTransmitter();    // read RC transmitter via receiver and 
  			// will put reading in rudderIn and sailwinchIn in degrees

 

/*-------------------------------------------------------------------*/
/*----------------------PROGRAM LOGIC -----------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

  if (manualRudder == true) 
      {rudderCMD = rudderIn;}
  else
    // close Haul Code for rudder
    // assume maximum position of rudder is +-80.  
    // to turn upwind rudder comand has opposite sign of relWind
    // Greater than +-135 push rudder full towards sail 
    // Within +-45 leave rudder alone (may want to add bear off code)
    // in reach zones create a linear function with slope -80/90
  {
    if (relWind <= -135) 
      rudderCMD = 80;
    if ((relWind > -135) && (relWind < -45)) 
      rudderCMD = (-.89 * relWind) - 40;
    if ((relWind >= -45) && (relWind <= 45))
      rudderCMD = 0;
    if ((relWind > 45) && (relWind < 135)) 
      rudderCMD = (-.89 * relWind) + 40;
    if (relWind >= 135) 
      rudderCMD = -80;
  }
  
  if (manualSail == true) 
      {sailCMD = sailwinchIn;}
  else
  {
  // close Haul Code for sail
    // assume maximum position of sail is +-90.  
    // to turn upwind rudder comand has opposite sign of relWind
    // Greater than +-135 push rudder full towards sail 
    // Within +-45 leave rudder alone (may want to add bear off code)
    // in reach zones create a linear function with slope -80/90// trim sail based on rudder position   
  // set to "full in' if within 45 of upwind and "full out" if within 45 of downwind
  // When the sail is in between make a linear relationship
  // between the sail setting and the wind
    absvalRelWind = abs(relWind);
    if (absvalRelWind < 45)
      sailCMD = 0;    
    if ((absvalRelWind >= 45) && (absvalRelWind < 135))
      sailCMD = absvalRelWind - 45;
    if (absvalRelWind >= 135)
      sailCMD = 90;
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
  if (servoname == rudder) rudderServo.write(pos+90); //servos go 0 to 180
  if (servoname == sailwinch) sailWinchServo.write(pos+90);
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
if (!((rCH < 1000) || (rCH > 2000)))  //noisy/out of range, don't change any values
  rCH = map(rCH, 1000,2000,-80,80);     // put reading in degrees, only go to 80 max
 else rCH = 99;
  sCH = pulseIn(sailwinch, HIGH, 25000);
if (!((sCH < 1000) || (sCH > 2000)))
    sCH = map(sCH, 1000,2000,0,90);
  else sCH = 99;
if (verbose)
  {
  Serial.print("Rudder from TxRx: ");   Serial.print(rCH); Serial.print("  |  ");
  Serial.print("SailWinch from TxRx:");  Serial.println(sCH);
  }
 rudderIn = rCH;
 sailwinchIn = sCH;
 return;
}


/*----------------- FUNCTION TO READ WIND SENSORS--------------------*/

float getRelativeWind()	  //read MA3 PWM output
{
  duration = pulseIn(windpin, HIGH);
  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 1010, -180, 180);

  // constrain value between 0 and 360 to get rid of noisy outliers
  // use minus sign so values increase in CCW direction
  WindDirection = -min(max(WindDirection, -180), 180);
  
  if (verbose) {
    Serial.print("Relative wind direction:------------------------ ");
    Serial.println(WindDirection); Serial.println();}
  return WindDirection;
}


/*----------------- USEFUL FUNCTIONS ------------------------------*/

float normalize0To360(float angle)
{
  while (angle < 0)  { angle += 360; }
  while (angle > 360){ angle -= 360; }
  return angle;
}

float normalizeN180ToP180(float angle)
{
  while (angle < -180){ angle += 360; }
  while (angle > 180){ angle -= 360;  }
  return angle;
}

