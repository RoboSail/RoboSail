// Sailservotest
// Diane Brancazio diane@robosail.org
/* This code tests a servo connected to digital pin 9
On the robosail boats that is the Sailwinch servo
It typically runs from 90 (full in) to 180 (full out)
The user is aksed for a position to send the servo to 
and the program sens the servo there.
It is useful to find the servo limits and intermediate positions
This Start with postions around 135 and move up/down from there
Record your values and use them in creating sailing algorithms
*/

#include <Servo.h> 
 
Servo sailservo; // create servo object to control a servo 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
   Serial.begin(9600); // Sets up communication with the serial monitor
  sailservo.attach(9);
} 
 
 void loop()
{
  Serial.println("Enter a position for sailservo to go to: ");
  while(Serial.available()==0) {} // Loop here until there is a character in the serial monitor
  int pos = Serial.parseInt();
  Serial.println(pos);

  sailservo.write(pos);  // tell servo to go to position in variable 'pos' 
 } 
