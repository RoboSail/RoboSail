How to use Electronic Compass for RoboSail boats
using Adafruit LSM303 3-axis magnetometer and 3-axis accelerometer
 * basic operation to verify hardware and connections
 * hard-iron calibration program to get values for other programs
 * tilt-compensated operation

In the "Libraries/Orientation" folder:

Orientation.cpp - Library for tilt calibrated compass readings
Orientation.h - Header file for library
compassTest - Example of how to use Orientation library
compassCalibration - Code to use to determine hard iron calibration

In the "hardware/compass" folder:

compassBasic - Raw accelerometer and magnetometer readings
compassBasicwithCalibration - hard iron and axis orientation correction
compassBasicwithCalibrationandTilt - roll, pitch, yaw, and heading


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

Because of the way the calculations are done, the heading will not be
accurate if the pitch approaches 90 or -90, but that shouldn't be a
problem (unless the boat is sinking it shouldn't be pointing straight up
or down). Otherwise, I think the heading should be accurate, even if the
boat is healing up to 75 degrees or so in either direction.

Thanks to:
Benjamin Kraus, ben@kraus.info
Cody Lewis, srlm@srlmproductions.com