#include "TemperatureReading.h"

bool heaterStop = false;
int setPointABS = 250;
int setPointPETG = 230;
int setPointPLA = 200;
int setPointPETE = 390;

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
float GetTemperature()
{
  int val[3] = {analogRead(tempPin1), analogRead(tempPin1), analogRead(tempPin1)};
  float voltage = 0;

  float temp[3] = {};
  for (int i = 0; i < 3; i++)
  {
    voltage = val[i] * (4.72 / 1023.0);
    temp[i] = (voltage - 1.25) / 0.005;
  }

  if(CompareSensorReading(temp[0], temp[1], temp[2]) == true)
  {
    float tempAvg = (temp[0] + temp[1] + temp[2]) / 3;
    tempAvg = round(tempAvg * 100.0) / 100.0;
    return tempAvg;
  }
  else
  {
    // TODO: stop the heater and check the connection of temperature sensor
    heaterStop = true;
  }
  return 0;
}

// Making the sure the 3 temperature sensor have similar readings. The difference  needs to be within 5 degree celcius
bool CompareSensorReading(float temp1, float temp2, float temp3)
{
  if((abs(temp1 - temp2) && abs(temp1 - temp3) && abs(temp2 - temp3)) <= 5)
  {
    return true;
  }
  return false;
}