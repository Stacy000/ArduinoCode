#include "Heater.h"
#include "TemperatureReading.h"

//Define Variables
double Setpoint, Input1, Output1;
double Input2, Output2;

//Specify the links and initial tuning parameters
double Kp1=300, Ki1=0.1, Kd1=0;
double Kp2=700, Ki2=0.5, Kd2=0;

// Define the two PID controllers
PID PID1(&Input1, &Output1, &Setpoint, Kp1, Ki1, Kd1, DIRECT);
PID PID2(&Input2, &Output2, &Setpoint, Kp2, Ki2, Kd2, DIRECT);

int windowSize = 5000;
volatile unsigned long windowStartTime;
bool steadyState = false;
int steadyCount = 0;

// Define variables
volatile unsigned long lastPID = 0;
volatile unsigned long lastSaveTime = 0;

// Function to turn on heaters
void TurnOnHeater()
{
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}

// Function to turn off heaters
void TurnOffHeater()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
}

// Set up all neccesary parameters for PID controller
void SetUpPID()
{
  digitalWrite(relay1, LOW); //tip
  digitalWrite(relay2, LOW); //end
  digitalWrite(relay3, LOW); //middle

  windowStartTime = millis();

  // // Define the set point for the PID controller
  // Setpoint = 610;

  //tell the PID to range between 0 and the full window size
  PID1.SetOutputLimits(0, windowSize); 
  PID2.SetOutputLimits(0, windowSize); 

  //turn the PID on
  PID1.SetMode(AUTOMATIC);
  PID2.SetMode(AUTOMATIC);
}

// Run the PID controller
void RunPID(int Setpoint)
{
  Input1 = analogRead(tempPin1);
  Input2 = analogRead(tempPin2);

  unsigned long currentMillis = millis();

  if(currentMillis - lastPID >= 4000)
  {
    PID1.Compute();
    PID2.Compute();
    //PID3.Compute();
    lastPID=currentMillis;
  }

  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if (currentMillis - windowStartTime > windowSize)
  {
    //time to shift the Relay Window
    windowStartTime += windowSize;
  }

  if (Output1 >= currentMillis - windowStartTime) 
  {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay3, HIGH);
    //Serial.println("high");
  }
  else 
  {
    digitalWrite(relay1, LOW);
    digitalWrite(relay3, LOW);
    //Serial.println("low");
  }

  if (Output2 >= currentMillis - windowStartTime) 
  {
    digitalWrite(relay2, HIGH);
    //Serial.println("high");
  }
  else 
  {
    digitalWrite(relay2, LOW);
    //Serial.println("low");
  }

  // Get the data for time, output, relay state and input, and check if the system has reach steady state
  if ((currentMillis - lastSaveTime) >= 1000) 
  {
    if(abs(Input1 - Setpoint)<=5 && abs(Input2 - Setpoint)<=5 && abs(Input1 - Input2)<=5)
    {
      steadyCount++;
    }

    // The system has reached steady state for 2 min
    if(steadyCount >= 120)
    {
      steadyState = true;
    }

    //Serial.print("Temperature: ");
    Serial.print(millis());
    Serial.print("  ");
    Serial.print(Output1);
    Serial.print("  ");
    Serial.print(digitalRead(relay1));
    Serial.print("  ");
    Serial.print(Input1);
    Serial.print("  ");

    Serial.print(Output2);
    Serial.print("  ");
    Serial.print(digitalRead(relay2));
    Serial.print("  ");
    Serial.println(Input2);

    lastSaveTime = currentMillis;
  }
}