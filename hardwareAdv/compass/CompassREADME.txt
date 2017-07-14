How to use Electronic Compass for RoboSail boats
using Adafruit LSM303 3-axis magnetometer and 3-axis accelerometer

Find these programs in the "hardware --> compass" folder and use in this order:

1. compassBasic - verify hardware and connections prints raw accelerometer and magnetometer readings to the screen
2. compassCalibration - Code to use to determine hard iron calibration values for other the programs
3. compassBasicwithCalibration - same as CompassBasic with hardiron correction
4. compassTest - Tilt-compensated compass with hard-iron correction.  Uses the Orientation library
5. compassBasicwithCalibrationandTilt - same as CompassTest, but does the tilt-compensation in the code and does NOT require the Orientation library

Find Orientation libraries in the folder Libraries --> Orientation:
	Orientation.cpp - Library for tilt calibrated compass readings
	Orientation.h - Header file for library

The roll, pitch, yaw, and headings all use the standard (Aircraft)
orientations:
https://en.wikipedia.org/wiki/Aircraft_principal_axes

A positive roll angle lifts the left wing and lowers the right wing.
A positive pitch angle raises the nose and lowers the tail.
A positive yaw, or heading angle, moves the nose to the right.

A yaw of 0 degrees is pointing towards Magnetic North.

Heading ranges from 0 to 360 degrees.
A heading of 0 degrees is pointing towards True North.
A heading of 90 degrees is pointing East, 180 is South, 270 is West.

For RoboSail we change to a different frame of reference where East is 0 deg,
North is 90 deg, West ia 180 deg, and South is 270 deg. To get this do 
RoboSailHeading = (360 - heading) + 90
We also use roll to find the heel of the boat.  
We define Positive roll as leaning to Port and Negative roll as leaning to Starboard.
To get this simply negate roll.  RoboSailRoll = -1 * roll

Because of the way the calculations are done, the heading will not be
accurate if the pitch approaches 90 or -90, but that shouldn't be a
problem (unless the boat is sinking it shouldn't be pointing straight up
or down). We found the compass readings are accurate to about +-5% even if the
boat is heeling at angles it would see in normal sailing.  
This accuracy sounds good, but realize that on 360 deg, it is up to +-18 deg.

Thanks to:
Benjamin Kraus, ben@kraus.info
Cody Lewis, srlm@srlmproductions.com