// Rudderservotest
// Diane Brancazio diane@robosail.org
/* This code tests a servo connected to digital pin 8
On the robosail boats that is the Rudder servo
It typically runs from 40 (full CW) to 140 (full CCW), center 90
The user is aksed for a position to send the servo to 
and the program sens the servo there.
It is useful to find the servo limits and intermediate positions
Start with postions around 90 and move up/down from there
Record your values and use them in creating sailing algorithms
*/
#include <Servo.h> 
 
Servo rudderservo; // create servo object to control a servo 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
   Serial.begin(9600); // Sets up communication with the serial monitor
  rudderservo.attach(8); // attaches the servo on pin 8 to the servo object 
} 
 
 void loop()
{
  Serial.println("Enter a position for rudderservo to go to: ");
  while(Serial.available()==0) {} // Loop here until there is a character in the serial monitor
  int pos = Serial.parseInt();
  Serial.println(pos);
pos = map(pos, -90, 90, 0, 180);
  rudderservo.write(pos);  // tell servo to go to position in variable 'pos' 

 } 
