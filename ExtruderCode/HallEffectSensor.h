#include <Arduino.h>

#define LED_hallEffect 30
#define hallEffectSensor A4

// Get the hall effect sensor reading
double GetDiameter();

// Check if the diameter is within the tolerance
bool CheckDiameter(double currentDiameter);