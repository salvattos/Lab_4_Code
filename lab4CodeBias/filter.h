#include <Zumo32U4.h>
#include <Wire.h>
#ifndef __FILTER_H
#define __FILTER_H

class Filter
{ //for variables and functions
  public:
    uint8_t statA;

    float predictedAngle;
    float lastAngle = 0;
    float lastBias = 0;
    float k = .5;
    float accOffset;
    float accTemp = 0;

    float eps = .01;
    int inc = 0;
    bool startCalc = 0;
   Filter();
    void Init();
    bool calcAngle(float& observedAngle, float& fusedAngle, float& bias);

};

#endif
