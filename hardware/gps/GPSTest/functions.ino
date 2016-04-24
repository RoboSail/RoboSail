void getposition()
{
   if (!(start_pos_found))
     {
        // take in lat/lon degree values and return (x,y) in meters in pos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, pos);
        startPositionX = pos[0];
        startPositionY = pos[1];
        
        Serial.println("Starting position found!");
        Serial.print("x = "); Serial.print(startPositionX);
        Serial.print("   y = "); Serial.println(startPositionY);
        Serial.print("  angle from start = "); Serial.println(angleFromStart);
        Serial.println();
        
        start_pos_found = true;
      }
      
      else // starting position was found
      {
        // take in lat/lon degree values and return (x,y) in meters in pos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, pos);
        
        // calculate the boat position relative to where it was started
        relPositionX = pos[0] - startPositionX;
        relPositionY = pos[1] - startPositionY;
        angleFromStart = atan2(relPositionY, relPositionX) * 180 / 3.14;
        while (angleFromStart < 0){ angleFromStart += 360; }
        while (angleFromStart > 360){ angleFromStart -= 360; }

        Serial.print("x = "); Serial.print(relPositionX);
        Serial.print("   y = "); Serial.println(relPositionY);
      }
}

