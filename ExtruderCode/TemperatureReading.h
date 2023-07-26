#include <Arduino.h>
//Define pins and variables for temperature reading
#define tempPin1 A0
#define tempPin2 A1
#define tempPin3 A2

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
float GetTemperature();