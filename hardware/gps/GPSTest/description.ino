/* GPSTest rev 4/22/2016
© 2014-2016 RoboSail
This program reads the GPS module through the Arduino Mega. 
It also test the libraries that convert the GPS latitude/longitude data (in degrees) 
into x,y coordinates (in meters).
The user can determine the information that is printed to the Serial Monitor
by setting variables in the code to true or false.

Use the code to see that the wiring and the code libraries are set up correctly.
When the GPS first gets a "fix" on position, that position is defined as the 0,0.
Future positions are relative to (0,0) as on an x,y plane. 
To change the (0,0) take the GPS to another spot and press the reset button  to a new (Need to rdefine the  on the Uno) control signals coming in from the Receiver and

Set Board to "Arduino Mega" for this code to compile
To run this code with the Arduino Uno, change "Serial1" to "Serial" 

*/
