Servo Test README file
© 2014-2016 RoboSail

Servo motors are used to drive the rudder and sail on an RC sailboat. 
Servo motors take in a PWM (pulse width modulated) signal and go to a 
position specified by the length of the pulse.  Tey typically have 180 
degrees of rotation and go from position 0 to position 180.

These programs are used to test the servo motors in the RC sailboat.  
The user can choose a position as specified in the RoboSail fram of reference. 

For RudderServoTest the user types in the desired angle through the Serial Monitor.
-50 means the rudder is fully to port, and +50 means the rudder
is fully to starboard.

Note that the Rudder Servo actual center postion is 90, so
the user's desired angles must be mapped in the code to these values.
   
The Rudder servo is controlled on digital pin 8

ForSailServoTest the user types in the desired angle through the Serial Monitor.
0 means the sail is fully in, and 90 means the sail is fully out (at 90 degrees).

Note that the Sail Servo actual positions for full in and out 
for 1 boat are approx 55 and 125, and they may vary for other boats.
Use this code to check these values and adjust as necessary, then map the user's 
desired angles to these values.    
   
The Sail servo is controlled on digital pin 9