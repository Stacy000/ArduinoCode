#include <Arduino.h>

//Define pins and variables for temperature reading
//#define tempPin3 A0
#define tempPin1 A1
#define tempPin2 A2

/* Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
   Return the average temperatrure of the 3 sensors if CompareSensorReading returns true
   Return 0 if CompareSensorReading returns false and Stops the heater
   */
double GetTemperature();

// Return true if the difference in the readings of the 3 temperature sensors is within 5 degree celcius; false otherwise
bool CompareSensorReading(double temp1, double temp2);