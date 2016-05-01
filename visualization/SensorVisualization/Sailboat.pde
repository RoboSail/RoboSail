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
  private Arrow windDirection;
  private float rudderAngleDeg;
  
  Sailboat(PVector location)
  {
    this.location = location;
    windDirection = new Arrow(
      new PVector(0, 0, 50), // will be replaced with value from sensor
      color(128, 255, 128));
  }
  
  // draws a sailboat model with origin at the center of the hull
  void draw()
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
    scale(0.2, 1, 1.8); // squish the shape into a sail
    truncCone.draw(0, 10, sailHeight, 16);
    popMatrix();
    popStyle();
    
    // draw the rudder
    pushStyle();
    fill(color(240, 240, 0));
    pushMatrix();
    translate(0, -rudderHeight, -20);
    rotateY(radians(rudderAngleDeg)); // rotate by the rudder angle
    scale(0.1, 1, 0.4); // squish the shape into a rudder
    truncCone.draw(10, 5, rudderHeight, 16);
    popMatrix();
    popStyle();
      
    popMatrix();
  }
  
  // draw the sailboat with roll/pitch/yaw arguments
  // roll, pitch, and yaw in degrees
  // TODO: clean this up, move roll/pitch/yaw rotations outside of sailboat class
  void drawRPY(float roll, float pitch, float yaw)
  {
    pushMatrix();
    noStroke();
  
    fill(color(220, 120, 0));
  
    translate(location.x, location.y, location.z);
    
    // roll: rotation about the Z axis
    rotateArb(radians(roll), 0, 0, 1);
  
    // pitch: rotation about the X axis
    rotateArb(radians(pitch), 1, 0, 0);
  
    // yaw: rotation about the Y axis
    rotateArb(radians(yaw), 0, 1, 0);
  
    draw();
    
    translate(0, 180, 0); // draw the arrow slightly above the sailboat
    windDirection.draw();
  
    popMatrix();
  }
  
  void setWindDirection(float angleDeg)
  {
    float angleRad = radians(angleDeg);
  
    // we want a vector in the X-Z plane, which points in the -Z direction for 
    // an angle of 0 degrees and points in the -X direction for an angle of 90 degrees
    PVector vector = new PVector(-sin(angleRad), 0, -cos(angleRad));

    // we also want a magnitude of 50
    vector.mult(100);
    
    windDirection.setVector(vector);
  }
  
  void setRudderAngle(float rudderAngleDeg)
  {
    this.rudderAngleDeg = rudderAngleDeg;
  }
}
