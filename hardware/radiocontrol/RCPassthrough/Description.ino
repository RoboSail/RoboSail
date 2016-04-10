/* RCPassthrough rev 4/8/2016
© 2014-2016 RoboSail
This program puts the Arduino micro-computer in the RC (Radio Control)system 
It takes in the control signals coming in from the Receiver and
pass them back to the Rudder and Sail servo motors on the boat.
It also displays several values to the Serial Monitor:
  - The actual "pulse" coming in from the receiver for each channel
    (typical range of 1000 - 2000)
  - the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 

This program helps the user determine 
  - if they are reading good signals from the receiver (range of 1000 - 2000)
  - if the servos are functioning correctly and moving to the expected positions
  - if the Arduino computer is functioning correctly

Rudder and Sail data from the RC receiver are read in on 
digital pins 2 and 3 respectively.
Rudder and sail servos are controlled on digital 
pins 8 and 9 respectively.
*/
