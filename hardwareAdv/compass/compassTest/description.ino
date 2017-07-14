/* CompassTest rev 7/22/2015
© 2014-2016 RoboSail
Tilt-compensated compass with hard-iron correction.  
Uses the Orientation library created for roboSail
Standard compass headings range from 0 to 360 degrees.
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

*/
