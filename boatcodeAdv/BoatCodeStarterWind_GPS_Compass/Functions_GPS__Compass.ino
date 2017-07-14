
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  GPS.read(); // reads char (if available) into internal buffer in GPS object
}

// function to enable TIMER0 interrupt for GPS
void enableInterrupt() {
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void readGPS()  //gets GPS data, parses it, and returns (x,y) position in meters in array called pos[]
{
if (GPS.newNMEAreceived())
  {
    char* LastNMEA; // declare pointer to GPS data
    LastNMEA = GPS.lastNMEA(); // read the string and set the newNMEAreceived() flag to false
    if (!GPS.parse(LastNMEA)) 
    {
      return; // failed to parse a sentence (was likely incomplete) so just wait for another
    }
    Serial.println("\nNew data from GPS");
    GPSfix = GPS.fix;  //put parsed data in variables for printing
    GPSqual = GPS.fixquality;
    GPSsat = GPS.satellites;
    if (GPS.fix)
    {
      if (start_pos_found)
      {
        // take in lat/lon degree values and return (x,y) in meters in XYpos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, XYpos);
        
        // calculate the boat position relative to where it was started
        relPositionX = XYpos[0] - startPositionX;
        relPositionY = XYpos[1] - startPositionY;
        angleFromStart = atan2(relPositionY, relPositionX) * 180 / 3.14;
        while (angleFromStart < 0){ angleFromStart += 360; }
        while (angleFromStart > 360){ angleFromStart -= 360; }

      }
      else // starting position not yet found but there is a fix
      { 
        // take in lat/lon degree values and return (x,y) in meters in pos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, XYpos);
        startPositionX = XYpos[0];
        startPositionY = XYpos[1];
        
        Serial.println("Starting position found!");
        Serial.print("x = "); Serial.print(startPositionX);
        Serial.print("   y = "); Serial.println(startPositionY);
        Serial.println();
        
        start_pos_found = true;
      }
    }
  }
}

void readCompassAccel()  //reads Compass to get heading and tilt
{
  float ax, ay, az, mx, my, mz;
  float my_adj, mx_adj;
  float Pi = 3.1415926;

   /* Get a new sensor event */
  sensors_event_t accel_event;
  accel.getEvent(&accel_event);
  sensors_event_t mag_event;
  mag.getEvent(&mag_event);

  /* Invert X so that when when X, Y, or Z is pointed down, it has a positive reading. */
  ax = -accel_event.acceleration.x;
  ay = accel_event.acceleration.y;
  az = accel_event.acceleration.z;
  
  /* Adjust for hard iron effects */
  mx = mag_event.magnetic.x - hardiron_x;
  my = mag_event.magnetic.y - hardiron_y;
  mz = mag_event.magnetic.z - hardiron_z;

  /* Invert Y and Z axis so that when X, Y, or Z is pointed towards Magnetic North they get a positive reading. */
  my = -my;
  mz = -mz;
  
  roll = atan2(ay,az);
  pitch = atan(-ax/sqrt(pow(ay,2)+pow(az,2)));
  
  my_adj = mz*sin(roll) - my*cos(roll);
  mx_adj = (mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll));
  
  yaw = atan2(my_adj,mx_adj);
  
  roll = roll * 180/Pi;
  pitch =  pitch * 180/Pi;
  yaw = yaw * 180/Pi;
  
  heading = yaw + declination;
  
  if (heading >= 360) {
    heading -= 360;
  } else if (heading < 0) {
    heading += 360;
  }
 //The heading is converted to a frame of reference for RoboSail:
 // East is 0 degrees, North is 90 deg, West is 180 deg, South is 270 deg. 
  robosailHeading = (360 - heading) + 90;
  if (robosailHeading >= 360) {robosailHeading -= 360;}
  
  //define roll for RoboSail as rolling to Port side is positive, rolling to Starboard is negative
  robosailRoll  = -1 * roll; 
}
