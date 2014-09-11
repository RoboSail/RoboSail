#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

class Orientation {
 public:
  static void calculate(Adafruit_LSM303_Accel_Unified * accl, Adafruit_LSM303_Mag_Unified * magn,
                 float hardiron_x, float hardiron_y, float hardiron_z,
                 float & roll, float & pitch, float & yaw);
  static float yawToHeading(float yaw, float declination);
};