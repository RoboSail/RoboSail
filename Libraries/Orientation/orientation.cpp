#include <orientation.h>

#define RAD_CONV 57.2957 // 180/Pi, to convert radians to degrees

/** Use an eCompass algorithm to calculate orientation
 * 
 * The algorithm is based on http://cache.freescale.com/files/sensors/doc/app_note/AN4248.pdf
 * 
 * Hardiron calibration must be performed. The process is simple:
 *   1. Mount the magnetometer in the location that you intend to use it at
 *   2. Rotate the body through all possible orientations
 *   3. Record the minimum and maximum for each axis of the magnetometer
 *   4. Average the minumum and maximum for each axis. This will give you your hardiron x,y,z offsets.
 *
 * @param accl The LSM303 acceleration object
 * @param magn The LSM303 magnetometer object
 * @param hardiron_x
 * @param hardiron_y
 * @param hardiron_z
 * @param roll The result roll in degrees
 * @param pitch The result pitch in degrees
 * @param yaw The result yaw in degrees
 */
void Orientation::calculate(Adafruit_LSM303_Accel_Unified * accl, Adafruit_LSM303_Mag_Unified * magn,
                 float hardiron_x, float hardiron_y, float hardiron_z,
                 float & roll, float & pitch, float & yaw){
  
  // Get a new sensor event
  sensors_event_t event_accl; 
  sensors_event_t event_magn; 

  accl->getEvent(&event_accl); 
  magn->getEvent(&event_magn);

  // Signs choosen so that, when axis is down, the value is + 1g
  float accl_x = -event_accl.acceleration.x;
  float accl_y = event_accl.acceleration.y;
  float accl_z = event_accl.acceleration.z;

  // Signs should be choosen so that, when the axis is down, the value is + positive.
  // But that doesn't seem to work ?...
  float magn_x = event_magn.magnetic.x + hardiron_x;
  float magn_y = -event_magn.magnetic.y + hardiron_y;
  float magn_z = -event_magn.magnetic.z + hardiron_z;  
  
  // Freescale solution
  roll = atan2(accl_y, accl_z);
  pitch = atan(-accl_x / (accl_y * sin(roll) + accl_z * cos(roll)));
  
  float magn_fy_fs = magn_z * sin(roll) - magn_y*cos(roll);
  float magn_fx_fs = magn_x * cos(pitch) + magn_y * sin(pitch) * sin(roll) + magn_z * sin(pitch) * cos(roll);
  
  yaw = atan2(magn_fy_fs, magn_fx_fs);
  
  roll = roll * RAD_CONV;
  pitch = pitch * RAD_CONV;
  yaw = yaw * RAD_CONV;
}

/** Convert a yaw (-180 to 180) to a compass heading (0 to 360) with declination correction
 * 
 * @param yaw The input yaw, in degrees -180 to 180
 * @param declination the magnetic declination to adjust against, with sign. Note that this is time and location dependent.
 * @return the corrected heading, in degrees 0 to 360
 */
float Orientation::yawToHeading(float yaw, float declination){
  float heading = yaw;
  if(yaw < 0.0){
    heading = 360.0 + yaw; // This is really a subtraction, since yaw is negative at this point
  }
  return heading + declination;
}