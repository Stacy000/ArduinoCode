#include "MainStepper.h"

// Define variables
double pulsePerRev = 200;
double rpm = 30;
unsigned long previousTime = 0;

// Get pulse period
double GetSleepTime() {
  double pulsePerSec = (rpm / 60) * pulsePerRev;
  return 1000/pulsePerSec;
}

void RunMainStepper(double sleepTime)
{
  //Set the direction of the main motor
  digitalWrite(directionNeg, LOW);
  digitalWrite(enableNeg, HIGH);

  unsigned long currentTime = millis();
  //Serial.print("hi");
  if (digitalRead(pulseNeg) == HIGH && currentTime - previousTime >= sleepTime) 
  {
    digitalWrite(pulseNeg, LOW);
    //Serial.println(digitalRead(pulseNeg));
  }

  if (currentTime - previousTime >= (2*sleepTime)) {
    // Generate a pulse
    digitalWrite(pulseNeg, HIGH);
    //Serial.println(digitalRead(pulseNeg));
    previousTime = currentTime;
  }  
}