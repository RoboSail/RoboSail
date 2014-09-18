 /*
Sensors And Their Data Outputs

Wind Vane (direction wind is coming from) 
  US Digital MA3 Magnetic Encoder
  Data comes in 0-360°, set 0° = in irons, starboard wind is 90° and port wind is -90°
  Relative reading

Compass (direction boat is pointing)
  Data comes in 0 to 360°, expect North is 0° but orient sensor so East = 0° 
  Adafruit LSM303 Accelerometer and Compass Breakout
  Absolute reading

GPS (location in latitude and longitude) 
  starts as angle in degrees, we convert to radians, then to x, y (meters)
  Adafruit Ultimate GPS Breakout
  Absolute reading
  
Absolute Wind Direction (real direction wind is coming from)
  Set East = 0° range = -180° to +180°
  Data needed: Relative Wind Direction
  
Absolute Angle-to-Waypoint (direction from boat to destination) 
  range is -180° to +180° or 0 to 360°
  Starting point is (x1, y1), destination waypoint is (x2, y2)
  Data needed: GPS location of boat and destination, or just difference
  
Relative Angle-to-Waypoint (angle boat must turn through to be heading at destination) 
  range is -180° to +180° or 0 to 360°
  Data needed: 
    o GPS location of boat and destination (or just difference)
    o Compass heading
    
Rudder
  clockwise = negative
  counter-clockwise = positive
  
Sail
  depends on the wind, but limit is 90 degrees
*/

//Include libraries for sensors and servos
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //set address

#define rudderServo 6
#define sailWinchServo 7  

//CHECK IF CHANNEL IS CORRECT 
//Transmitter channel values
int ch1; 
int ch2;

#define SERVOMIN  250 // minimum pulse length count for servo
#define SERVOMAX  500 // maximum pulse length count for servo
#define rudderGAIN -.5 // multiply angles by this to get rudderCMD

// From wind sensor (degrees)
float relWind = 0;              
int rudderCMD = 0;
//sailCMD stays positive
int sailCMD = 45;
#define ironsRange 45

#define rudder 11    //TODO input pins from receiver
#define sailwinch 12
int rudderIn;	// inputs from transmitter, in degrees
int sailwinchIn;
int pulselen; //PWM signal to servos
#define windpin 7   //Wind sensor input pin
unsigned long duration;
int WindDirection;

int tackDegree; 
 
void setup() 
{
  /*----------------------SET UP WIND SENSOR---------------------------*/
  // WindSensor MA3 setup code - set pin as digital input
  pinMode(windpin, INPUT);
  
  /*----------------------SET UP SERVOS---------------------------*/
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  /*----------------------SET UP SERIAL MONITOR---------------------------*/
  Serial.begin(9600);
  
  /*----------------------SET UP INPUT FROM TRANSMITTER ------------------*/
  rudderIn =  0; //TODO understand
  sailwinchIn = 45; //TODO understand
}

void loop() 
{  
  //read Transmitter and put reading in rudderIn and sailwinchIn in degrees, 
  readTransmitter();	
  //getRelativeWind() function is below
  relWind = getRelativeWind();  
  
  // rudderGAIN is set above and should be negative, range is 75 degr

  //sets the limits for the rudder movement
  rudderCMD = max(min(relWind * rudderGAIN, 75), -75);
    
  //BUFFER: dont make changes if within 5 degrees
  if (abs(rudderCMD)< 5 ) 
  {
    rudderCMD = 0;
  }

  tackDegree = closeHaul() * 90;
  delay(15000);
  tack(tackDegree);
}

/*----------------- FUNCTION TO TACK ------------------*/
void tack(int boatTurn)
{
  
  //boatTurn is positive when boat turns counter-clockwise and negative when boat turns clockwise
  int newRelWind = relWind - boatTurn;
  while (newRelWind < -180)
  {
    newRelWind += 180;
  }
  while (newRelWind > 180)
  {
    newRelWind -= 180;
  }
  
  //Port Tack
  if (relWind > 0)
  {
    while( relWind > newRelWind)
    {
      commandMotor(-75, rudderServo); //steer rudder to maximum clockwise
      commandMotor(60, sailWinchServo); // let the sail luff a bit, roughly 60 degrees
      relWind = getRelativeWind();
    }
  }
  //Starboard Tack
  else if (relWind < 0)
  {
    while( relWind < newRelWind)
    {
      commandMotor(75, rudderServo);
      commandMotor(60, sailWinchServo);
      relWind = getRelativeWind();
    }
  }
  
  commandMotor(0, rudderServo);
  trimSail(relWind);
  
  //alternative to trimSail?
  //if boatTurn is negative, then sail ADDS 0.5 * boatTurn
  //if boatTurn is positive, then sail SUBTRACTS 0.5 * boatTurn
  //commandMotor(abs(sailCMD-(0.5*boatTurn)), sailWinchServo);
}

/*----------------- FUNCTION TO TRIM SAIL (SET IT TO MOVE WITH WIND) ------------------*/
void trimSail(int relativeWind)
{
  // trim sail based on rudder position   
  // set to "full in' if within 45 of upwind and "full out" if within 45 of downwind
  // When the sail is in between make a linear relationship
  // between the sail setting and the wind
  if (abs(relativeWind) < ironsRange)
  {  
    sailCMD = 0;
  }
  if (abs(relativeWind) > 135)
  {  
    sailCMD = 90;
  }
  if ((abs(relativeWind) > ironsRange) && (abs(relativeWind) < 135))
  {  
     sailCMD = abs(relativeWind) - 45;
  }
  
  commandMotor(sailCMD, sailWinchServo);
}

/*----------------- FUNCTION TO CLOSE HAUL EITHER DIRECTION ------------------*/
int closeHaul()
{
  //determine which is the closest closeHaul
  int newRelWind;
  
  //returns which direction (+90 or -90) to tack depending on which closeHaul
  int portOrStarboardWind;
  
  //INSTANCE 1: closest closeHaul would be the boat is sailing toward the right, wind at port
  //make relWind = 45
  if(relWind >= 0 && relWind <= 180)
  {
    portOrStarboardWind = 1; //the next tack will be positive 90 degrees
    newRelWind = 45;
    if (relWind > newRelWind)
    {
      while(relWind > newRelWind)
      {
        commandMotor(-75, rudderServo);
        relWind = getRelativeWind();
      }
    }
    else //(relWind < newRelWind)
    {
      while(relWind < newRelWind)
      {
        commandMotor(75, rudderServo);
        relWind = getRelativeWind();
      }
    }
  }
  //INSTANCE 2: closest closeHaul would be if the boat is sailing toward the left, wind at starboard
  //make relWind = -45
  else if (relWind <= 0 && relWind >= -180)
  {
    portOrStarboardWind = -1; //the next tack will be negative 90 degrees
    newRelWind = -45;
    if(relWind < newRelWind)
    {
      while(relWind < newRelWind)
      {
        commandMotor(-75, rudderServo);
        relWind = getRelativeWind();
      }
    }
    else //(relWind > newRelWind)
    {
      while(relWind > newRelWind)
      {
        commandMotor(75, rudderServo);
        relWind = getRelativeWind();
      }
    }
  }
  
  commandMotor(0, rudderServo);
  //trimSail(relWind);
  return portOrStarboardWind;
}














//challenge was to program a robotic sailboat
//write basic manuevers
//3 sensors

//control jibe, pull sail in, turn a little and wind will catch, and let sail out gradually

//snap jibe, let sail all the way out, turn 90 degrees let wind catch the sail and swing the sail















/*----------------- FUNCTIONS TO COMMAND THE MOTORS------------------*/
void commandMotor(int pos, int servoname)
 {
  int pulselen = 375;
  
  if (servoname == rudderServo) 
  {
    if (pos >= 0) 
    {
      pulselen = (map(pos, 0, 80, 375, SERVOMAX));  // TODO  change range
    }
    
    if (pos < 0) 
    {
      pulselen = (map(pos, 0, -80, 375, SERVOMIN));  // TODO   change range
    }
    //creates PWM signal starting from 0 and going to pulselen for servo connected at "servonum"
    pwm.setPWM(servoname, 0, pulselen);
  }
    
  if (servoname == sailWinchServo)
  {
    pulselen = (map(pos, 0, 90, SERVOMIN, SERVOMAX));  // change range

    //creates PWM signal starting from 0 and going to pulselen for servo connected at "servonum"
    pwm.setPWM(servoname, 0, pulselen);
  }
}


/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
void readTransmitter()
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
if ((rCH < 1000) || (rCH > 2000)) return;	//don't change any values
  rCH = map(rCH, 1000,2000,-80,80);		// put reading in degrees
 
  sCH = pulseIn(sailwinch, HIGH, 25000);
if ((sCH < 1000) || (sCH > 2000)) return;
    sCH = map(sCH, 1000,2000,0,90);

 rudderIn = rCH;
 sailwinchIn = sCH;
}

/*----------------- FUNCTION TO READ THE WIND SENSOR ------------------*/
float getRelativeWind()
{
  //read MA3 PWM output
  duration = pulseIn(windpin, HIGH);
  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 995, -180, 180); // TODO Changed
  WindDirection = -min(max(WindDirection, -180), 180);
  return WindDirection;
}
