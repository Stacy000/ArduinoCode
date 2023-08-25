#include "HallEffectSensor.h"

bool DiameterOK = false;

double GetDiameter() {
  int hallEffectVal = analogRead(hallEffectSensor);

  // Remap the value for output. In my experiments, the minimum input
  // value for when the north part of the magnet was directly in front of 
  // the Hall effect sensor was ~22
  // int ledVal = map(hallEffectVal, 520, 715, 0, 255);

  // // Print the raw photocell value and the converted led value (e,g., for Serial 
  // // Console and Serial Plotter)
  // Serial.print(hallEffectVal);
  // Serial.print(",");
  // Serial.println(ledVal);
  
  // // Write out the LED value. 
  // analogWrite(LED_hallEffect, ledVal);

  // delay(20); // read at 50Hz
  return hallEffectVal;
}

bool CheckDiameter(double currentDiameter)
{
  // TODO: calibrate the diameter sensor, 
  // get the hall effect sensor reading when the filament of 1.75mm is goint through the diameter sensor 
  // convert the raw reading to diameter and compare if the current diameter measured by the sensor is within +-0.1mm of 1.75mm.
}