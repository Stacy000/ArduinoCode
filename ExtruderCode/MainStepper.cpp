#include "MainStepper.h"

// Define variables
int pulsePerRev = 200;
int rpm = 30;
unsigned long previousTime = 0;

// Get pulse period
float GetSleepTime() {
  float pulsePerSec = rpm / 60 * pulsePerRev;

  return 1000/pulsePerSec;
}

// void RunMainStepper()
// {
//   //Set the direction of the main motor
//   digitalWrite(directionNeg, HIGH);
//   digitalWrite(enableNeg, HIGH);

//   unsigned long currentTime = millis();
//   //Serial.print("hi");
//   if (digitalRead(pulseNeg) == HIGH && currentTime - previousTime >= 10) 
//   {
//     digitalWrite(pulseNeg, LOW);
//     Serial.println(digitalRead(pulseNeg));
//   }

//   if (currentTime - previousTime >= 20) {
//     // Generate a pulse
//     digitalWrite(pulseNeg, HIGH);
//     Serial.println(digitalRead(pulseNeg));
//     previousTime = currentTime;
//   }  
//}