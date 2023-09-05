#include "TemperatureReading.h"

bool heaterStop = false;
int setPointABS = 230;
int setPointPETG = 230;
int setPointPLA = 200;
int setPointPETE = 270;

int setPointABSRaw = 450; //230 deg C
int setPointPETGRaw = 490; 
int setPointPLARaw = 460; //200 deg C
int setPointPETERaw = 530; //270 deg C

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
double GetTemperature()
{
  int val[3] = {analogRead(tempPin1), analogRead(tempPin2), analogRead(tempPin3)};
  float voltage = 0;

  double temp[3] = {};
  for (int i = 0; i < 3; i++)
  {
    voltage = val[i] * (5 / 1023.0);
    temp[i] = (voltage - 1.25) / 0.005;
  }
  
  //if(CompareSensorReading(temp[0], temp[1]) == true)
  //{
    double tempAvg = (temp[0] + temp[1]+ temp[2]) / 3;
    tempAvg = round(tempAvg * 100.0) / 100.0;
    return tempAvg;
  //}
}

// Making the sure the 2 temperature sensor have similar readings. The difference  needs to be within 10 degree celcius
bool CompareSensorReading(double temp1, double temp2)
{
  if(abs(temp1 - temp2) <= 10)
  {
    return true;
  }
  return false;
} 
