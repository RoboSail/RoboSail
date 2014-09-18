/*CloseHaul
7/24/2014   Diane Brancazio
reads Wind direction sensor and uses it to make boat sail closehaul
added code for tack
prints commands to serial monitor if asked
using 16-ch PWM Servo Shield 
Connect servos to servo slots 6 (rudder) and 7 (sailWinch)
*/

//Include libraries for sensors and servos

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //set address

#define rudderServo 6
#define sailWinchServo 7  
int ch1; // Transmitter channel values
int ch2;
#define SERVOMIN  250 // minimum pulse length count for servo
#define SERVOMAX  500 // maximum pulse length count for servo

int relWind = 0;      // From wind sensor (degrees)}
int absvalRelWind; 	// absolute value of wind sensor
int rudderCMD = 0;
int sailCMD = 45;
#define ironsRange    45

#define rudder 11    //input pins from receiver
#define sailwinch 12
int rudderIn;	// inputs from transmitter, in degrees
int sailwinchIn;
int pulselen; //PWM signal to servos
#define windpin       7   //Wind sensor input pin
unsigned long duration;
int WindDirection;

bool verbose = true;  //Sends messages to the Serial Monitor when true
bool manualSail = false; //set to false for autonomous sail
bool manualRudder = false;  //set to false for autonomous rudder

bool wind_good = false;   //not currently used
 
void setup() 
{

  /*----------------------SET UP WIND SENSOR---------------------------*/
  // WindSensor MA3 setup code - set pin as digital input
  pinMode(windpin, INPUT);
  wind_good = true;
  /*----------------------SET UP SERVOS---------------------------*/
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  /*----------------------SET UP SERIAL MONITOR---------------------------*/
  Serial.begin(9600);
  Serial.println("CloseHaul starting up...");
  /*----------------------SET UP INPUT FROM TRANSMITTER ------------------*/
  rudderIn =  0;
  sailwinchIn = 45;
}

void loop() 
{
  if (verbose) Serial.print("\n\n");
  //read Transmitter and put reading in rudderIn and sailwinchIn in degrees, 
  readTransmitter();	
  //read wind sensor
  relWind = getRelativeWind();    
  // Set rudder to get to close haul
  // rudderGAIN is set above and should be negative, range is 75 degr
  
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
  if (verbose)
   	{Serial.print("rudderCMD = "); Serial.println(rudderCMD);}
  
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
     {Serial.print("SailCMD = "); Serial.println(sailCMD);}

//COMMAND THE MOTORS

// will send input from transmitter or calculated postion 
//depending on whether it is manual or autonomous
commandMotor(rudderCMD, rudderServo);  
commandMotor(sailCMD, sailWinchServo);

} // End of loop()



/*----------------- FUNCTIONS TO COMMAND THE MOTORS------------------*/


void commandMotor(int pos, int servoname)
 {
  int pulselen = 375;
  if (servoname == rudderServo) 
  {
	if (pos >= 0) pulselen = (map(pos, 0, 80, 375,SERVOMAX));  // change range
	if (pos < 0) pulselen = (map(pos, 0, -80, 375,SERVOMIN));  // change range
  	Serial.print("driving rudderServo to position: "); 
  Serial.println(pos); 
  Serial.println(pulselen); 
  //creates PWM signal starting from 0 and going to pulselen for servo connected at "servonum"
  	pwm.setPWM(servoname, 0, pulselen);
  }
  
  if (servoname == sailWinchServo)
  {
	pulselen = (map(pos, 0, 90,SERVOMIN,SERVOMAX));  // change range
  	Serial.print("driving sailWinchServo to position: "); 
  	Serial.println(pos); 
	Serial.println(pulselen); 

  	//creates PWM signal starting from 0 and going to pulselen for servo connected at "servonum"
  	pwm.setPWM(servoname, 0, pulselen);
  }
}


/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
if ((rCH < 1000) || (rCH > 2000)) return;	//don't change any values
  rCH = map(rCH, 1000,2000,-80,80);			// put reading in degrees
 
  sCH = pulseIn(sailwinch, HIGH, 25000);
if ((sCH < 1000) || (sCH > 2000)) return;
    sCH = map(sCH, 1000,2000,0,90);
if (verbose)
  {
  Serial.print("Rudder from transmitter: "); 
  Serial.println(rCH);
  Serial.print("SailWinch from Transmitter:");
  Serial.println(sCH);
  }
 rudderIn = rCH;
 sailwinchIn = sCH;
 return;
}

/*----------------- FUNCTION TO READ THE WIND SENSOR ------------------*/
int getRelativeWind()
{
  //read MA3 PWM output
  duration = pulseIn(windpin, HIGH);

  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 1010, -180, 180);

  // constrain value between 0 and 360 to get rid of noisy outliers
  // use minus sign so values increase in CCW direction
  WindDirection = -min(max(WindDirection, -180), 180);
  
  if (verbose) {
    Serial.print("Relative wind direction: ---------------- ");
    Serial.println(WindDirection);
  }

  return (WindDirection);
}
/* -------------- FUNCTION TO TACK ---------------------*/
void tack(int newRelWind)
// Take in a desired position for the wind sensor, 
// should >= 45 or <= -45
{
  if (relWind > 0)  // positive wind so currently on a port tack
  {
    while (relWind > newRelWind)
    {
      commandMotor(-75, rudderServo);   //turn rudder hard towards sail
      commandMotor(60, sailWinchServo); //let sail luff during tack
      relWind = getRelativeWind();      // new value in relWind
    }
  }
  if (relWind <= 0)  // negative wind so currently on a starboard tack
  {
    while (relWind < newRelWind)
    {
      commandMotor(75, rudderServo);   //turn rudder hard towards sail
      commandMotor(60, sailWinchServo); //let sail luff during tack
      relWind = getRelativeWind();      // new value in relWind
    }
  } 
      commandMotor(0, rudderServo);   //turn rudder hard towards sail
      commandMotor(0, sailWinchServo); //let sail luff during tack
      // need some delay or check to make sure servos get there
}
