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
  private float sailAngleLimitDeg;
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
    pushStyle();
    noStroke();
    pushMatrix();
    scale(4, 4, 4);
    drawHull();
    drawSail();
    drawRudder();
    popMatrix();
    popStyle();
  }
  
  void drawSail()
  {
    float sailHeight = 30;
    float sailAngleDeg = getRealSailAngle();
    TruncCone truncCone = new TruncCone();

    pushStyle();
    if (sailAngleDeg == windAngleDeg)
    {
      // the sail is luffing - tint red
      fill(color(255, 180, 180));
    }
    else
    {
      // not luffing - show white
      fill(color(240, 240, 240));
    }
    pushMatrix();
    translate(0, 8, 0);
    
    // rotate by the sail angle
    translate(0, 0, 15);
    rotateY(radians(getRealSailAngle())); 
    translate(0, 0, -15);
    
    scale(0.2, 1, 1.8); // squish the shape into a sail
    truncCone.draw(0, 10, sailHeight, 16);
    popMatrix();
    popStyle();
  }
  
  void drawHull()
  {
    float hullHeight = 10;
    TruncCone truncCone = new TruncCone();

    pushStyle();
    fill(color(220, 120, 0));
    pushMatrix();
    translate(0, -hullHeight/2, 0); // sink down into the water by half the hull
    scale(1, 1, 5); // stretch the cylinder into a hull
    truncCone.draw(5, 3, hullHeight, 16);  
    popMatrix();
    popStyle();
  }
  
  void drawRudder()
  {
    float rudderHeight = 20;
    TruncCone truncCone = new TruncCone();

    pushStyle();
    fill(color(240, 240, 0));
    pushMatrix();
    translate(0, -rudderHeight, -20);
    
    // rotate by the rudder angle
    translate(0, 0, 5);
    rotateY(radians(rudderAngleDeg));
    translate(0, 0, -5);
    
    scale(0.1, 1, 0.4); // squish the shape into a rudder
    truncCone.draw(10, 5, rudderHeight, 16);
    popMatrix();
    popStyle();
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
  
  // see notes on sailAngleDeg - here, we calculate the "real" sail angle, 
  // based on wind direction
  float getRealSailAngle()
  {
    // get the wind angle in the range of -180 to 180
    float windAngleDegMod = normalizeAngle(windAngleDeg);
    if (windAngleDegMod >= 0)
    {
      // port tack - sail should go out to the starboard side (positive angle)
      // also, limit the sail to the angle of the wind
      return (windAngleDegMod < sailAngleLimitDeg) ? windAngleDegMod : sailAngleLimitDeg;
    }
    else
    {
      // starboard tack - sail should go out to the port side (negative angle)
      // also, limit the sail to the angle of the wind
      return (windAngleDegMod > -sailAngleLimitDeg) ? windAngleDegMod : -sailAngleLimitDeg;
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
      println("Sailboat: Bad rudder angle set: " + rudderAngleDeg);
    }
  }
  
  void setSailAngleLimit(float sailAngleLimitDeg)
  {
    if (sailAngleLimitDeg >= 0 && sailAngleLimitDeg <= 90)
    {
      this.sailAngleLimitDeg = sailAngleLimitDeg;
    }
    else
    {
      println("Sailboat: Bad sail angle limit set: " + sailAngleLimitDeg);
    }
  }
}
