#include <Arduino.h>

// Define the pins
#define pulseNeg 7
#define directionNeg 8
//int enableNeg = 10;

// Get pulse period
int getSleepTime(int pulsePerRev, int rpm);
void RunMainStepper();
