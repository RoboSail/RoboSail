/*
 * MatrixUtils
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This file contains miscellaneous helper functions for matrix math.
 */
import processing.opengl.*;

// applies a general rotation of r radians around a vector specifed by the x, y, and z 
// arugments
void rotateArb(float r, float x, float y, float z) 
{  
  float d = sqrt(x*x + y*y + z*z);  
  float a = x/d, b = y/d, c = z/d;  
  float t[][] = {
    {
      a*a, a*b, a*c
    }
    , {
      b*a, b*b, b*c
    }
    , {
      c*a, c*b, c*c
    }
  };  
  float s[][]= {
    {
      0, -c, b
    }
    , {
      c, 0, -a
    }
    , {
      -b, a, 0
    }
  };  
  float cosr = cos(r);  
  float sinr = sin(r); 
  float m[][] = {
    {
      1, 0, 0
    }
    , {
      0, 1, 0
    }
    , {
      0, 0, 1
    }
  };  

  for (int i = 0; i < 3; i++)    
    for (int j = 0; j < 3; j++)	  
      m[i][j] = t[i][j] + cosr*(m[i][j] - t[i][j]) + sinr*s[i][j];  

  applyMatrix(m[0][0], m[0][1], m[0][2], 0, 
    m[1][0], m[1][1], m[1][2], 0, 
    m[2][0], m[2][1], m[2][2], 0, 
    0, 0, 0, 1);
} 
