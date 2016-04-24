void driveSailServo(int sailPos)
{
  if ((sailPos >= 0) && (sailPos <= 90))  // the command in degrees is valid
  {
    sailPos = map(sailPos, 0, 90, 55, 125);
    sailServo.write(sailPos);
  }
  else 
  {
    Serial.print("ERROR - sail position out of range: ");
    Serial.println(sailPos);
    }
}

