
/*RCpassThru and Serial Read out
Diane Brancazio diane@robosail.org
revised 10/11/14
/reads in signal from radio control receiver 
/prints it to serial monitor, 
/maps it to approp servo range and outputs it to servo to make servo move
uses on-board PWM facilities
Connections on Grove Seeeduino Mega shield which has servo connectors
Cut V trace on board for separate power source to servo bus.
Grounds stay connected
Connect receiver to servo slots 11 (rudder STR), 12, (sail THR) 
Connect servos to servo slots 8 (rudder) and 9 (sailWinch)
*/
  #include <Wire.h>
  #include <Servo.h>				// for servos

  bool verbose = true;	//Set to true to enable printlns to the Serial Monitor

  Servo rudderServo;  //create servo objects
  Servo sailWinchServo; 
  #define rudder      10  //input pins from receiver
  #define sailwinch   11
  #define rservo 8
  #define sservo 9

  int rudderIn; // inputs from transmitter, in degrees
  int sailwinchIn;
  int rudderCMD = 0;  // outputs to servos
  int sailCMD = 45;
  bool RC_good = false;			//not currently used

 
void setup() 
{
  Serial.begin(9600);
Serial.print("RCPassThru code starting.   ");
  pinMode(rudder, INPUT);	  // Set RC receiver on digital input pins
  pinMode(sailwinch, INPUT);
  RC_good = true;	//need to test for this?
  
  rudderServo.attach(rservo);
  sailWinchServo.attach(sservo);   // attaches the servo to appropriate pin 

}
 
void loop() 
{

  readTransmitter();    // read RC transmitter via receiver and 
  			// will put reading in rudderIn and sailwinchIn in degrees
  rudderCMD = rudderIn;
  sailCMD = sailwinchIn;

  if (rudderCMD !=99) commandMotor(rudderCMD, rudder);  // will send input from transmitter 
  else Serial.println("Rudder command from Tx error");
  if (sailCMD !=99) commandMotor(sailCMD, sailwinch);
  else Serial.println("Sail command from Tx error");
  delay(500);// put on delay to slow serial monitor but also slows servo response  
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
  sCH = pulseIn(sailwinch, HIGH, 25000);

if (verbose)
  {
  Serial.print("Rudder raw (from Tx):        ");   Serial.print(rCH);
  Serial.print("     SailWinch raw (from Tx):    ");  Serial.println(sCH);
  }  
  if ((rCH < 1000) || (rCH > 2000)) rCH = 99; //noisy/out of range, send flag,don't change any values
  else rCH = map(rCH, 1000,2000,-80,80);     // put reading in degrees, only go to 80 max
 
  if ((sCH < 1000) || (sCH > 2000)) sCH = 99;  //noisy/out of range, send flag, don't change any values
   else sCH = map(sCH, 1000,2000,0,90);     // put reading in degrees, only go to 80 max
if (verbose)
  {
  Serial.print("Rudder Commmand in degrees:    ");   Serial.print(rCH);
  Serial.print("      SailWinch Command in degrees: ");  Serial.println(sCH);
//   Serial.println();
  }
 rudderIn = rCH;  //  if error flag comes through then don't change values
 sailwinchIn = sCH;
 return;
}

/*----------------- FUNCTION TO COMMAND A MOTOR------------------*/

void commandMotor(int pos, int servoname)
 {
 	//rudder pos comes in as -80 to +80, servo goes 0 to 180 with center at 0, so add 90
  if (servoname == rudder) 
  {
    rudderServo.write(pos+90);  
    if (verbose)  {  Serial.print("Rudder Actual (servo pos):    ");   Serial.print(pos+90); }
  }
  if (servoname == sailwinch) 
  {
    	//sail pos comes in as 0-90, servo goes 0 to 180 with center at 90, so add 90
    sailWinchServo.write(pos+90); 
  if (verbose)   { Serial.print("      SailWinch Actual (servo pos): ");  Serial.println(pos+90); 
   Serial.println();}
  }
}

