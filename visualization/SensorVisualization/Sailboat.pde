/*
 * Sailboat
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a 3D model of a sailboat, including a "floating" arrow to represent
 * wind direction.  It contains a function to draw it with a particular roll, pitch, and yaw.
 */
import processing.opengl.*;

public class Sailboat
{
  private PVector location;
  private float windAngleDeg;
  private Arrow windDirection;
  private float rudderAngleDeg;
  // this should always be a positive number - the sail angle set by the client code
  // should be between 0 and 90 degrees, as the client only really knows how far the 
  // the sheet has been let out - within this class, we will calculate the "real" 
  // sail angle, based on the wind direction
  private float sailAngleDeg;
  private float rollDeg;
  private float pitchDeg;
  private float yawDeg;
  
  Sailboat(PVector location)
  {
    this.location = location;
    windDirection = new Arrow(
      new PVector(0, 0, 50), // will be replaced with value from sensor
      color(128, 255, 128));
  }
  
  // draws a sailboat model with origin at the center of the hull
  void drawBasic()
  {
    float sailHeight = 30;
    float hullHeight = 10;
    float rudderHeight = 20;
    TruncCone truncCone = new TruncCone();
  
    noStroke();
    
    pushMatrix();
    scale(4, 4, 4);
  
    // draw the hull
    pushStyle();
    fill(color(220, 120, 0));
    pushMatrix();
    translate(0, -hullHeight/2, 0); // sink down into the water by half the hull
    scale(1, 1, 5); // stretch the cylinder into a hull
    truncCone.draw(5, 3, hullHeight, 16);  
    popMatrix();
    popStyle();
  
    // draw the sail
    pushStyle();
    fill(color(240, 240, 240));
    pushMatrix();
    translate(0, hullHeight/2 + hullHeight/3, 0);
    translate(0, 0, 15);
    rotateY(radians(getRealSailAngle())); // rotate by the sail angle
    translate(0, 0, -15);
    scale(0.2, 1, 1.8); // squish the shape into a sail
    truncCone.draw(0, 10, sailHeight, 16);
    popMatrix();
    popStyle();
    
    // draw the rudder
    pushStyle();
    fill(color(240, 240, 0));
    pushMatrix();
    translate(0, -rudderHeight, -20);
    translate(0, 0, 5);
    rotateY(radians(rudderAngleDeg)); // rotate by the rudder angle
    translate(0, 0, -5);
    scale(0.1, 1, 0.4); // squish the shape into a rudder
    truncCone.draw(10, 5, rudderHeight, 16);
    popMatrix();
    popStyle();
      
    popMatrix();
  }
  
  // draw the sailboat, accounting for location, roll/pitch/yaw, and wind arrow
  void draw()
  {
    pushMatrix();
    noStroke();
  
    translate(location.x, location.y, location.z);
    
    // roll: rotation about the Z axis
    rotateArb(radians(rollDeg), 0, 0, 1);
  
    // pitch: rotation about the X axis
    rotateArb(radians(pitchDeg), 1, 0, 0);
  
    // yaw: rotation about the Y axis
    rotateArb(radians(yawDeg), 0, 1, 0);
  
    drawBasic();
    
    translate(0, 180, 0); // draw the arrow slightly above the sailboat
    windDirection.draw();
  
    popMatrix();
  }
  
  void setWindDirection(float angleDeg)
  {
    windAngleDeg = angleDeg;
    
    float angleRad = radians(angleDeg);
  
    // we want a vector in the X-Z plane, which points in the -Z direction for 
    // an angle of 0 degrees and points in the -X direction for an angle of 90 degrees
    PVector vector = new PVector(-sin(angleRad), 0, -cos(angleRad));

    // we also want a magnitude of 50
    vector.mult(100);
    
    windDirection.setVector(vector);
  }
  
  float getRealSailAngle()
  {
    // see notes on sailAngleDeg - here, we calculate the "real" sail angle, 
    // based on wind direction
    float windAngleDegMod = windAngleDeg % 360;
    if (windAngleDegMod >= 0 && windAngleDegMod < 180) // math is TBD
    {
      // port tack - sail should go out to the starboard side
      return sailAngleDeg;
    }
    else
    {
      // starboard tack - sail should go out to the port side
      return -sailAngleDeg;
    }
  }
  
  void setRPY(float rollDeg, float pitchDeg, float yawDeg)
  {
    this.rollDeg = rollDeg;
    this.pitchDeg = pitchDeg;
    this.yawDeg = yawDeg;
  }
  
  void setRudderAngle(float rudderAngleDeg)
  {
    if (rudderAngleDeg >= -90 && rudderAngleDeg <= 90)
    {
      this.rudderAngleDeg = rudderAngleDeg;
    }
    else
    {
      println("Sailboat: Bad rudder angle set:" + rudderAngleDeg);
    }
  }
  
  void setSailAngle(float sailAngleDeg)
  {
    if (sailAngleDeg >= 0 && sailAngleDeg <= 90)
    {
      this.sailAngleDeg = sailAngleDeg;
    }
    else
    {
      println("Sailboat: Bad sail angle set:" + sailAngleDeg);
    }
  }
}
