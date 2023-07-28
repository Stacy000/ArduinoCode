#include "MainStepper.h"

// Define variables
int pulsePerRev=200;
int rpm=30;
int sleepTime;
unsigned long lastStepTime = 0;

// Get pulse period
int getSleepTime(int pulsePerRev, int rpm) {

  int pulsePerSec=rpm/60*pulsePerRev;
  int sleepTime=1/pulsePerSec;
  return sleepTime;
}

void RunMainStepper(int sleepTime)
{
  //Set the direction of the main motor
  digitalWrite(directionNeg, HIGH);

  unsigned long currentMillis = millis();

  if ((currentMillis - lastStepTime) >= sleepTime) 
  {
    digitalWrite(pulseNeg, HIGH);
    //delayMicroseconds(sleepTime);
    digitalWrite(pulseNeg, LOW);
    Serial.println(pulseNeg);
    //delayMicroseconds(sleepTime);
  } 
}