/*
 * Extrusion
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents an "interesting-looking" 3D animated "wavey" surface, along the
 * X-Z plane, with the specified X and Z length.
 */
import processing.opengl.*;

public class Extrusion
{  
  private final float resolution = 20; // the lower the number, the higher the level of detail in the model
  private float amp1 = 10;
  private float amp2 = 10;
  private float phase1 = 0;
  private float phase2 = 20;
  private float phase3 = 0;
  private float drawExtruAlpha = 128;
  private float xMax;
  private float zMax;
  
  Extrusion(float xMax, float zMax)
  {
    this.xMax = xMax;
    this.zMax = zMax;
  }

  void draw()
  {
    int aCount = int(xMax/resolution) + 1;
    int bCount = int(zMax/resolution) + 1;
    
    // the surfaces is essentialy just a 2-dimensional array of y-values
    // these y-valued points will be "connected up" later, via triangular surfaces
    PVector[][] yValues = new PVector[aCount][bCount];
    int a = 0, b = 0;
    float x, y, z;

    // for each point in the X-Z plane, calculate a y-value
    // the y-value is determined by the result of 3 animated waves running through our surface:
    // - a sine wave in the X direction
    // - a cosine wave in the Z direction
    // - a sine wave in the Z direction
    for (x = 0; x <= xMax; x += resolution)
    {
      for (z = 0; z <= zMax; z += resolution)
      {
        y = amp1 * sin(phase1 + radians(x)) * cos(phase2 + radians(z)) + amp2 * sin(phase3 + radians(z));
        yValues[a][b] = new PVector(x, y, z);
        b++;
      }
      a++;
      b = 0;
    }   
    
    // now that the y-values have been calculated, play "connect the dots", and connect each point to 
    // its surrounding points with triangles
    //noStroke();
    stroke(color(0, 0, 200));
    for (a = 0; a < aCount-1; a++)
    {
      beginShape(TRIANGLE_STRIP);
      for (b = 0; b < bCount-1; b++)
      {
        PVector v1 = yValues[a][b];
        PVector v2 = yValues[a+1][b];
        PVector v3 = yValues[a][b+1];
        PVector v4 = yValues[a+1][b+1];
    
        fillVertexColor(v1);
        vertex(v1.x, v1.y, v1.z);
        fillVertexColor(v2);
        vertex(v2.x, v2.y, v2.z);
        fillVertexColor(v3);
        vertex(v3.x, v3.y, v3.z);
        fillVertexColor(v4);
        vertex(v4.x, v4.y, v4.z);
      }
      endShape();
    }   
    
    animateSurface();
  }
  
  private void fillVertexColor(PVector v)
  {
    fill(0, 0, map(v.y, -100, 100, 128, 255), drawExtruAlpha);
  }
  
  private void animateSurface()
  {
    // animate the surface by changing the phase of the waves
    //amp1 += 0.001;
    phase1 += 0.005;
    phase2 += 0.005;
    phase3 += 0.005;
  }
}
