/*
 * TruncCone
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a 3D truncated cone.  This shape/class can be used generically to create
 * cones or cylinders.
 */
import processing.opengl.*;

public class TruncCone
{
  // draws in alignment with the Y axis, starting at origin and moving in the positive Y direction
  void draw(float topRadius, float bottomRadius, float tall, int sides) 
  {
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) 
    {
      vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
      vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
      angle += angleIncrement;
    }
    endShape();
  
    // if it is not a cone, draw the circular top cap
    if (topRadius != 0) 
    {
      angle = 0;
      beginShape(TRIANGLE_FAN);
  
      // Center point
      vertex(0, 0, 0);
      for (int i = 0; i < sides + 1; i++) 
      {
        vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
        angle += angleIncrement;
      }
      endShape();
    }
  
    // if it is not a cone, draw the circular bottom cap
    if (bottomRadius != 0) 
    {
      angle = 0;
      beginShape(TRIANGLE_FAN);
  
      // Center point
      vertex(0, tall, 0);
      for (int i = 0; i < sides + 1; i++) 
      {
        vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
        angle += angleIncrement;
      }
      endShape();
    }
  }
}
