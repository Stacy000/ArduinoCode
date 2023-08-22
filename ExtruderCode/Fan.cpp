#include "Fan.h"

void TurnOffFan()
{
  digitalWrite(fanIn1, LOW);
  digitalWrite(fanIn2, LOW);
  digitalWrite(fanIn3, LOW);
  digitalWrite(fanIn4, LOW);
}

void TurnOnFan()
{
  digitalWrite(fanIn1, HIGH);
  digitalWrite(fanIn2, LOW);
  digitalWrite(fanIn3, HIGH);
  digitalWrite(fanIn4, LOW);
}