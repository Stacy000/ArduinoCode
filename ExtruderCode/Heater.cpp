#include "Heater.h"

void TurnOnHeater()
{
  digitalWrite(relay1, HIGH);
}

void TurnOffHeater()
{
  digitalWrite(relay1, LOW);
}