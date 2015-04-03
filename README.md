RoboSail Project
=========

This project is an ongoing project under Boston Community Boating, [CBI] (http://www.community-boating.org), Junior program. For more information check out [robosail.org] (http://www.robosail.org/)!



## Contributing
If you're a team, and you want to test out your own code, please make a fork of the project and add your code there.

If you're just interested in contributing, please make your changes in a pull request for review.


## documents folder

Contains all of the related documents.

## Reading and Checking the Sensors

Include 

* WindSensorCalibration.ino in the WindSensorCalibration folder, used to calibrate the wind sensor.
* RCpassThru0906.ino in the RCpassThru0906 folder, used to read in signal from radio control receiver and prints it to serial monitor.

### WindSensorCalibration.ino

 Our wind sensor is attached to the top of the mast and will return a value from -180 to 180.
> Read and calibrate MA3 magnetic digital encoder Reads a PWM digital input on pin 7, prints the result to the serial monitor USe this to find the range of values from the sensor, then map it to degrees Prints value from sensor and calculated heading so dead spots can be identified.

### RCpassThru0906.ino

RCpassThru and Serial Read out
> reads in signal from radio control receiver prints it to serial monitor, maps it to approp servo range and outputs it to servo to make servo move uses on-board PWM facilities Connections on Grove Seeeduino Mega shield which has servo connectors Cut V trace on board for separate power source to servo bus. Grounds stay connected Connect receiver to servo slots 11 (rudder STR), 12, (sail THR) Connect servos to servo slots 8 (rudder) and 9 (sailWinch).

## Libraries

Contains all the library files needed. One should have in mind that a correct path should be given to the library in the Arduino IDE preferences, under the Sketchbook location!
