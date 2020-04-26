/* This example reads the raw values from the L3GD20H gyro and
  LSM303D accelerometer and magnetometer on the Zumo 32U4, and
  prints those raw values to the serial monitor.

  The accelerometer readings can be converted to units of g using
  the conversion factors specified in the "Sensor characteristics"
  table in the LSM303 datasheet.  The default full-scale (FS)
  setting is +/- 2 g, so the conversion factor is 0.61 mg/LSB
  (least-significant bit).  A raw reading of 16384 would correspond
  to 1 g.

  The gyro readings can be converted to degrees per second (dps)
  using the "Mechanical characteristics" table in the L3GD20H
  datasheet.  The default sensitivity is 8.75 mdps/LSB
  (least-significant bit).  A raw reading of 10285 would correspond
  to 90 dps.

  The magnetometer readings are more difficult to interpret and
  will usually require calibration. */

#include "filter.h";


Zumo32U4Motors motors;

Filter filterA;

char report[120];
float CONVERSION = 16384;
static float observed = 0;
static float fused = 0;
static float bias = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}
  filterA.Init();
}

void loop()
{
  if (filterA.calcAngle(observed, fused, bias)) {
    Serial.print(observed);
    Serial.print("\t");
    Serial.print(fused);
    Serial.print("\t");
    Serial.print(bias);
    Serial.print("\t");
    Serial.print(millis());
    Serial.println();
  }

}
