#include "filter.h"
LSM303 compass;
L3G gyro;
Zumo32U4ButtonC buttonC;
Zumo32U4ButtonB buttonB;
Filter::Filter() {

}

bool Filter::calcAngle(float& observedAngle, float& fusedAngle, float& gyroBias){ 
  statA = compass.readReg(LSM303::STATUS_A);
  if(statA & 0x0F){
    gyro.read(); //setup
    compass.readAcc();
    
    //calcs
    gyroReading = (gyro.g.y*.00875) + gyroBias;
    gyroBias = lastBias + eps*(observedAngle - predictedAngle); //update bias
    predictedAngle = lastAngle + (.01)*(gyroReading - lastBias); //prediction with bias correction
    observedAngle = (atan2((compass.a.x),(compass.a.z)) * (180.0/3.14259)) - accOffset; //Observation from accelerometer
    fusedAngle = k*predictedAngle + (1-k)*observedAngle; //correction
    


     if(buttonC.isPressed()){ //running average for acc offset
      startCalc = true;
      accTemp = 0;
      accOffset = 0;
     }
     if(startCalc){
      if(inc < 200){
        accTemp += observedAngle;
        inc++;
      }else{
        inc = 0;
        accOffset = accTemp/200.0;
        startCalc = false;
      }
     }
     if(buttonB.isPressed()) accOffset = 0; // for recalibration
    
    //update variables
    lastAngle = fusedAngle;
    lastBias = gyroBias;
    return 1;
  }else return 0;
}


void Filter::Init(void) {
  Wire.begin();
  if (!compass.init())
  {
    // Failed to detect the compass.
    ledRed(1);
    while (1)
    {
      Serial.println(F("Failed to detect the compass."));
      delay(100);
    }
  }

  compass.enableDefault();

  if (!gyro.init())
  {
    // Failed to detect the gyro.
    ledRed(1);
    while (1)
    {
      Serial.println(F("Failed to detect gyro."));
      delay(100);
    }
  }

  gyro.enableDefault();

  gyro.writeReg(L3G::CTRL1, 0xBF);
  compass.writeReg(LSM303::CTRL1, 0x67);
}
