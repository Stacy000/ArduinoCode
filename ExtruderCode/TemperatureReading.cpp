#include "TemperatureReading.h"

bool heaterStop = false;
int setPointABS = 250;
int setPointPETG = 230;
int setPointPLA = 200;
int setPointPETE = 390;

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
double GetTemperature()
{
  int val[3] = {analogRead(tempPin1), analogRead(tempPin2)};
  float voltage = 0;

  double temp[2] = {};
  for (int i = 0; i < 2; i++)
  {
    voltage = val[i] * (2.56 / 1023.0);
    temp[i] = (voltage - 1.25) / 0.005;
  }
  
  if(CompareSensorReading(temp[0], temp[1]) == true)
  {
    double tempAvg = (temp[0] + temp[1]) / 2;
    tempAvg = round(tempAvg * 100.0) / 100.0;
    return tempAvg;
  }
  else
  {
    // TODO: stop the heater and check the connection of thermocouple
    heaterStop = true;
  }
  return 0;
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