RoboSail Code
=========

This project is an ongoing project under Boston's Community Boating Inc. ([CBI] (http://www.community-boating.org)) Junior program. For more information check out [robosail.org] (http://www.robosail.org/).

The RoboSail program (www.robosail.org) intends to introduce high school students to robotics and engineering projects through a fun and challenging world of sailing. It was inspired by the university-level SailBot robotic sailboat competition and is being developed by a team of Boston Area teachers, engineers, students, professors and sailing enthusiasts. 

In 2014 we partnered with Boston's Community Boating, Inc. [CBI] (http://www.community-boating.org), to create a RoboSail class for the [Junior program] (http://www.community-boating.org/programs/junior-program/).

For more information  (http://www.robosail.org/cbi-robosail-class.html) 

# Installation

Comming Soon!

# Contributing
If you're a team, and you want to test out your own code, please make a fork of the project and add your code there.

If you're just interested in contributing, please make your changes in a pull request for review.

# Folder Overview

## documents Folder

Contains all of the related documents.

## hardware Folder

Includes folders for each major hardware component, with usage examples inside.

## boatcode Folder

Templates to help teams get up and running quickly with a complete sailboat program.

## Libraries Folder

Contains all the library files needed. Libraries contain hardware-specific code provided by the component manufacturer.  Simple components like the wind direction encoder send a PWM signal, which Arduino is already capable of interpreting, so no additional library is necessary.  Both the Compass and GPS modules come with libraries from the manufacturer and have libraries created by the RoboSail team for ease of use.

Make sure that all Libraries are in the Libraries folder in the Sketchbook location.   You can check/set this in the Arduino IDE preferences.

# Pinout and Interrupts

Pin | Assignment
----|-------------------------
2   | RC receiver sail
3   | RC receiver rudder
8   | Servo rudder
9   | Servo sail

Interrupt | Assignment
----------|-------------------------
0         | RC receiver pulse in sail
1         | RC receiver pulse in rudder


Arduino Pin Code

```
// Pin assignments


const int ROBOSAIL_PIN_RUDDER_RC = 3;
const int ROBOSAIL_PIN_SAIL_RC = 2;

const int ROBOSAIL_PIN_WIND = 7;

const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;

const int ROBOSAIL_INTERRUPT_SAIL = 0;
const int ROBOSAIL_INTERRUPT_RUDDER = 1;
```

# New Hardware Setup

This section only applies to people who want to follow along with their own hardware. If you're a RoboSail student you can skip this: it's already been done for you.

* Connections on Grove Seeeduino Mega shield which has servo connectors 
* Cut V trace on board for separate power source to servo bus. Grounds stay connected 
* Connect receiver to servo slots 11 (rudder STR), 12, (sail THR) 
* Connect servos to servo slots 8 (rudder) and 9 (sailWinch).


## Calibration and Tuning

These boat constants will differ slightly for each set of hardware used. Each team should record and save the constants that are unique to their boat.

Variable            | Measured Value 
--------------------|------------------
`RC_SAIL_MINIMUM`   | 
`RC_SAIL_MAXIMUM`   | 
`RC_RUDDER_MINIMUM` | 
`RC_RUDDER_MAXIMUM` | 
`SAIL_IN`           | 
`SAIL_OUT`          | 
`RUDDER_RIGHT`      | 
`RUDDER_LEFT`       | 
`WIND_MINIMUM`      | 
`WIND_MAXIMUM`      | 

Example values:

Variable            | Measured Value 
--------------------|------------------
`RC_SAIL_MINIMUM`   | 1092
`RC_SAIL_MAXIMUM`   | 1892
`RC_RUDDER_MINIMUM` | 1096
`RC_RUDDER_MAXIMUM` | 1924
`SAIL_IN`           | 1200
`SAIL_OUT`          | 1892
`RUDDER_RIGHT`      | 1156
`RUDDER_LEFT`       | 1874
`WIND_MINIMUM`      | 0
`WIND_MAXIMUM`      | 1024

To calculate the values and tune the boat follow these steps:

### Sail Servo

1. Run [RCReader.ino](hardware/radiocontrol/RCReader/RCReader.ino) and open a terminal to examine the measured values.
2. Center all trim sliders on the RC transmitter.
3. Move the left stick (the "throttle") all the way down. Record the sail value as `RC_SAIL_MINIMUM`.
4. Move the left stick all the way up. Record the sail value as `RC_SAIL_MAXIMUM`.
5. Next, we will tune the hardware of the sailboat. Make sure you have a hex key tool.
6. Loosen the mainsheet and jibsheet from their booms so that they can slide freely.
7. Run [RCPassthrough.ino](hardware/radiocontrol/RCPassthrough/RCPassthrough.ino) and open a terminal to examine the measured values.
8. Move the left stick all the way down so that the sheets are hauled in.
9. Record the RC value as `SAIL_IN`.
10. Fix the mainsheet and jibsheet attachments to their booms so that the sails are on the boat centerline.
11. Gradually move the left stick up until the mainsail is at 90 degrees from the boat.
12. Record the value as `SAIL_OUT`.

### Rudder Servo

1. Run [RCReader.ino](hardware/radiocontrol/RCReader/RCReader.ino) and open a terminal to examine the measured values.
2. Center all trim sliders on the RC transmitter.
3. Move the right stick (the "steering") all the way right. Record the rudder value as `RC_RUDDER_MINIMUM`.
4. Move the right stick all the way left. Record the rudder value as `RC_RUDDER_MAXIMUM`.
5. Next, we will tune the hardware of the sailboat. You do not need any tools.
6. Run [RCPassthrough.ino](hardware/radiocontrol/RCPassthrough/RCPassthrough.ino) and open a terminal to examine the measured values.
7. Move the right stick right until the rudder is at it's maximum deflection. Record the rudder value as `RUDDER_RIGHT`.
8. Move the right stick left until the rudder is at it's maximum deflection. Record the rudder value as `RUDDER_LEFT`.

### Wind Sensor

1. Run [WindSensor.ino](hardware/wind/WindSensor/WindSensor.ino) and open a terminal to examine the measured values.
2. Next, we will tune the hardware of the sailboat.
3. Rotate the wind vane until the rollover point is found (eg, from 0 to 1024).
4. Loosen the wind vane sensor, and rotate it until the rollover point is set directly forward.
5. Fix the wind vane sensor in place.
6. Measure the minimum value of the wind sensor. Record this value as `WIND_MINIMUM`.
7. Measure the maximum value of the wind sensor. Record this value as `WIND_MAXIMUM`.

#  VARIABLES defined as Absolute and Relative

What is meant by Relative and Absolute

Relative is relative to the boat. Wind at the nose is 0 degrees, and it goes from 0-180 on the starboard side and 0 to -180 on the port side. A relative boat direction of 90 means turning the boat 90 degrees to starboard

Absolute is relative to the world. East is 0, and it increases in the counter clockwise direction. If the boat is at (0, 0) and the next waypoint is (-20, 20), the next waypoint is at an absolute heading of 135 degrees, regardless of which direction the boat is pointing at the moment

# Technical Notes

## Parts of a sailboat

![source: sailfree.com](http://www.sailfree.com/sailboats/_derived/sailboat.htm_txt_parts.gif)


## Characterize the Servos
Use the programs Rudderservotest and Sailservotest to find the servo limits and intermediate positions of the Servos.  The program asks for a position to send the motors to, the user enters it, then the program send the motor to that position.
The Rudder servo typically runs from 40 (full CW) to 140 (full CCW), center 90. Start with positions around 90 and move up/down from there.

The Sailwinch servo typically runs from 90 (full in) to 180 (full out).  Start with positions around 135 and move up/down from there
Record your values and use them in creating sailing algorithms.

## Reading and Checking the RC Transmitter/Receiver

The Arduino Computer goes between the RC Transmitter/Receiver and the Servo Motors.  Use the program RCPassThru to check this communication.

The signal that is read in from the receiver is a  pulse read in with the pulseIn function. It typically goes from 1000 to 2000.   That number is mapped to the degrees that we define for the servo motion: The Rudder ranges -80 to +80 degrees with 0 at the center.  1000 maps to -80 while 2000 maps to +80 degrees. The Sailservo is a multiturn winch that we only use in 1 direction (so the line does not rewind in the other direction after unwinding). We define 0 degrees as the line (and the sail) is fully wound in and at 90 degrees the line (and the sail) is fully out. 1000 maps to 0 while 2000 maps to 90 degrees.

There is an additional step to create the proper command for the servo.  We need to create servo positions that correspond to the degree positions that we want.  Servos usually run from 0 to 180 degrees, however there may be physical limits that prevent full movement.  In this case, the rudder is a bit limited at the ends of its travel by the connecting rods.  The rudder servo has its center point at roughly 90 degrees.  The sail servo is at 90 when fully in, 90 when fully out, and 90 to 180 is not used as it would rewind the line.

Once the correct servo positions are calculated, they are output to the servos by on-board PWM facilities.

## Calibrate the Wind Sensor

Our wind sensor is attached to the top of the mast and will return a value from -180 to 180.

It uses a 10-bit magnetic digital encoder that creates a digital pulse, the length of which corresponds to its rotary position. The code reads this in using the pulseIn function then maps it to the desired range of -180 to 180 degrees. Both the pulse value and the mapped values are displayed to the serial Monitor.

Nominal values for the position are 0 to 1023. Use the code to determine if the actual range is smaller and there are "dead spots".  You can then change the map function accordingly.

## Compass: Calibrate and Read
For Compass Heading there is an electronic magnetometer board that also includes a 3-axis accelerometer - Adafruit LSM-303DLHC. A magnetometer can be calibrated and will work fine on flat surfaces away from magnetic fields, however, a boat heels significantly. Therefore a compass need to be tilt-compensated. Information from the boards 3-axis magnetometer and 3-axis accelerometer are combined in some tricky software to give a compass reading within +-5°.

Hardiron calibration must be performed. The process is simple:

1. Mount the magnetometer in the location that you intend to use it at
2. Rotate the body through all possible orientations
3. Record the minimum and maximum for each axis of the magnetometer
4. Average the minimum and maximum for each axis. This will give you your hardiron x,y,z offsets.
 
(Need more data here about tilt-compensation)

## GPS: Read

The GPS sensor can be mounted inside or outside the boat in such a way that it has a clear view of the sky with nothing shielding its view. The GPS provides location information in degrees latitude and longitude and updates this information every second.  To do this it must have a "clear view of the sky" and so it can get a fix from 3 satellites, (though it can work with only 1).  An additional antenna is used to  improve reception.  It must be mounted horizontal, but can be in the hull of the boat.  

Latitude and Longitude are read in degrees and minutes and cover the entire world.  Software has been provided (in the UsefulCalcs library) that converts the latitude and longitude degrees into a simple x, y grid using a method called UTM (Universal Transverse Mercator).  The accuracy is still only +-2 meters, but the numbers are much easier to use.

Need GPS part data and connections info.
