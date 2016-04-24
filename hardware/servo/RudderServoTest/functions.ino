void driveRudderServo(int rudderPos)
{
  if ((rudderPos >= -50) && (rudderPos <= 50))
  {
    rudderPos = map(rudderPos, -90, 90, 0, 180);
    rudderServo.write(rudderPos);
  }
  else 
    {
    Serial.print("ERROR - rudder position out of range: ");
    Serial.println(rudderPos);
    }
}
