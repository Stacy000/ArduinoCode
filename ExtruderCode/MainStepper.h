#include <Arduino.h>

// Define the pins
#define pulseNeg 4
#define directionNeg 5
#define enableNeg 7

// Get pulse period
double GetSleepTime();
void RunMainStepper(double sleepTime);
