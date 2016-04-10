RadioControl test README file
© 2014-2016 RoboSail
The 2-channel RadioControl (RC) Transmitter/Receiver pair come with the sailboat.
The signals are originally used to directly control the servos. When the RoboSail 
control system is installed, the signals from the Receiver are routed through the
Arduino, then processed before getting to the servo motors.  
To make automatic control, the outputs to the servo motors are based on the sensor 
inputs, not the user inputs.

When automatic control is implemented, the signals from the RC transmitter can 
be used to send switch signals or other cues to the boat to switch modes of operation.

Use these programs to 
  * verify the Transmitter/Receiver and getting good signals (range of 1000 - 2000)
  * Verify the system wiring and that the Arduino is functioning
  * Verify that the servos are functioning correctly and moving to the expected positions

RCReader only reads in signals to verify the Transmitter/Receiver
RCPassThrough reads in signals and passes them onto the servos as control signals

RCReader and RCPassThrough:
  - take in the control signals coming in from the Receiver and 
    display the actual "pulse" coming in from the receiver for each channel
    (typical range of 1000 - 2000)

RCPassThrough
  - creates servo commands as expected from the Receiver
  - displays the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 


Rudder and Sail data from the RC receiver are read in on 
digital pins 2 and 3 respectively.

Rudder and sail servos are controlled on digital 
pins 8 and 9 respectively.