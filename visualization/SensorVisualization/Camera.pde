/*
 * Camera
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a camera in 3D space, including support for smooth/animated
 * camera movements.
 */
import processing.opengl.*;

public class Camera
{
  private PVector location;
  private PVector lookAtTarget;
  private float distToLookAtTargetXZ;
  private float targetRotationYRad;
  private float rotationYRad;
  private final float animationFactor = 16; // higher number results in slower camera movements

  Camera(PVector location, PVector lookAtTarget)
  {
    this.location = location;
    this.lookAtTarget = lookAtTarget;
    
    distToLookAtTargetXZ = calcXZDistToLookTarget();
  }
  
  // this must be called at the beginning of the draw() function, before other objects are drawn
  void defineCamera()
  {
    // x to the right, y up, z into the camera
    camera(location.x, location.y, -location.z, lookAtTarget.x, lookAtTarget.y, -lookAtTarget.z, 0, -1, 0);
    scale(1, 1, -1);
    
    updateTargetRotationFromMouse();
    animateLocation();
  }
  
  private void updateTargetRotationFromMouse()
  {
    targetRotationYRad = map(mouseX, 0, width, -PI/6, PI * 4/6);
  }

  private void animateLocation()
  {
    // move in a circular pattern around the target, by the set speed of the camera
    rotationYRad += (targetRotationYRad - rotationYRad)/animationFactor;

    location.x = lookAtTarget.x + distToLookAtTargetXZ * cos(rotationYRad);
    location.y = location.y; // "no-op", but for clarity
    location.z = lookAtTarget.z + distToLookAtTargetXZ * sin(rotationYRad);
  }

  // calculates the distance from the camera to the target, based on the XZ plane only  
  float calcXZDistToLookTarget()
  {
    PVector lookAtTargetXZ = new PVector(lookAtTarget.x, 0, lookAtTarget.z);
    PVector locationXZ = new PVector(location.x, 0, location.z);
    PVector v = PVector.sub(locationXZ, lookAtTargetXZ);
    return v.mag();
  }
}

