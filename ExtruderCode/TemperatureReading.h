#include <Arduino.h>

//Define pins and variables for temperature reading
#define tempPin1 A0
#define tempPin2 A1
#define tempPin3 A2

/* Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
   Return the average temperatrure of the 3 sensors if CompareSensorReading returns true
   Return 0 if CompareSensorReading returns false and Stops the heater
   */
float GetTemperature();

// Return true if the difference in the readings of the 3 temperature sensors is within 5 degree celcius; false otherwise
bool CompareSensorReading(float temp1, float temp2, float temp3);