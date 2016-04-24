void initialDisplay()
{
  Serial.print("Your hardiron values are X: "); Serial.print(hardiron_x);
  Serial.print("  Y: "); Serial.print(hardiron_y); 
  Serial.print("  Z: "); Serial.println(hardiron_z); Serial.println();
}

void displayToScreen() // Display the results
{
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(", Pitch: "); Serial.print(pitch);
  Serial.print(", Yaw: "); Serial.print(yaw);
  Serial.print(",   Compass Heading: "); Serial.print(heading); Serial.print(" °");
  Serial.print(",   RoboSail Heading: "); Serial.print(robosailHeading); Serial.println(" °");
}
