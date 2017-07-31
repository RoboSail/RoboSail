/*********Functions for Setup ************/
void declarePins()
{
  pinMode(RUDDER_RC_PIN, INPUT);
  pinMode(SAIL_RC_PIN, INPUT);
  pinMode(WIND_PIN, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(RUDDER_SERVO_PIN);
  sailServo.attach(SAIL_SERVO_PIN);
}


/*********Functions to read RC Transmitter/Receiver and Sensors *****/


 
 void readReceiver()
        // Takes in the PWM signals from the RC Receiver and translates
        // translates them to the servo positions in degrees.
 {
  // Read the command pulse from the RC receiver
  rudderPulseWidth = pulseIn(RUDDER_RC_PIN, HIGH);
  sailPulseWidth = pulseIn(SAIL_RC_PIN, HIGH);
  // Calculate the servo position in degrees.
  rudderPosition = map(rudderPulseWidth, RUDDER_LOW, RUDDER_HIGH, -60, 60);
  sailPosition = map(sailPulseWidth, SAIL_LOW, SAIL_HIGH, 0, 90);
 }
 
 void readWind()
        // Takes in the PWM signals from the WindSensor and translate 
        // it to the windvane position in degrees.
 {
  // Read values from the WindSensor
  windPulseWidth = pulseIn(WIND_PIN, HIGH);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, WIND_HIGH, 180, -180);
  windAngle = constrain(windAngle, -180, 180);
 }

/************Functions to drive Sail and Rudder servos ****************/
 // This code takes in the desired postions for the servos in degrees (as 
 // defined in RoboSail) then calculates appropriate values for the servo commands, 
 // making sure not to send the servos to impossible positions, which could 
 // damage the servo motors.
 // The Rudder servo motor ranges from 0 to 180 with 90 deg in the center
 // The Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
 // and ~125 deg when full out, which we think of as 90 deg

void driveSailServo(int sailPos)
{
  if ((sailPos >= 0) && (sailPos <= 90))  // the command in degrees is valid
  {
    sailServoOut = map(sailPos, 0, 90, 55, 125);
    sailServo.write(sailServoOut);
  }
  else 
  {
    Serial.print("ERROR - sail position out of range: ");
    Serial.println(sailPos);
    }
}

void driveRudderServo(int rudderPos)
{
  if ((rudderPos >= -60) && (rudderPos <= 60))
  {
    rudderServoOut = map(rudderPos, -90, 90, 0, 180);
    rudderServo.write(rudderServoOut);
  }
  else 
    {
    Serial.print("ERROR - rudder position out of range: ");
    Serial.println(rudderPos);
    }
}
/****************************************************/
// Function to Print out all values for debug.
void printToMonitor()
{
  Serial.print("Wind Angle: ");
  Serial.print(windAngle);
 
  Serial.print("     Sail, RC: ");
  Serial.print(sailPulseWidth);
  Serial.print("  desired angle: ");
  Serial.print(sailPosition);
  Serial.print("  to servo: ");
  Serial.print(sailServoOut);
  
  Serial.print("  Rudder, RC: ");
  Serial.print(rudderPulseWidth);
   Serial.print("  desired angle: ");
  Serial.print(rudderPosition);
  Serial.print("  to servo: ");
  Serial.print(rudderServoOut);

  Serial.print("\n"); // Print a new line
}

