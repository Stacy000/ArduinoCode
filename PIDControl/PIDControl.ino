#include <PID_v1.h>

//Define pins and variables for temperature reading
//#define tempPin1 A0
#define tempPin2 A1
#define tempPin3 A2
#define relay1 7
#define relay2 50
#define relay3 51

//Define Variables
double Setpoint, Input1, Output1;
double Input2, Output2;
//double Input3, Output3;

//Specify the links and initial tuning parameters
double Kp1=300, Ki1=0.1, Kd1=0;
double Kp2=700, Ki2=0.5, Kd2=0;
PID PID1(&Input1, &Output1, &Setpoint, Kp1, Ki1, Kd1, DIRECT);
PID PID2(&Input2, &Output2, &Setpoint, Kp2, Ki2, Kd2, DIRECT);
//PID PID3(&Input3, &Output3, &Setpoint, Kp, Ki, Kd, DIRECT);


int WindowSize = 5000;
unsigned long windowStartTime;

// Define variables
volatile unsigned long lastPID = 0;
volatile unsigned long lastSaveTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReference(INTERNAL2V56); // Set the analog reference voltage to 2.56V

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  digitalWrite(relay1, LOW); //tip
  digitalWrite(relay2, LOW); //end
  digitalWrite(relay3, LOW); //middle

  Serial.println("Starting");
  windowStartTime = millis();

  // Define the set point for the PID controller
  Setpoint = 600;

  //tell the PID to range between 0 and the full window size
  PID1.SetOutputLimits(0, WindowSize); 
  PID2.SetOutputLimits(0, WindowSize); 
  //PID3.SetOutputLimits(0, WindowSize); 

  //turn the PID on
  PID1.SetMode(AUTOMATIC);
  PID2.SetMode(AUTOMATIC);
  //PID3.SetMode(AUTOMATIC);

  Input1 = analogRead(A1); //tip
  Input2 = analogRead(A2); //end
  //Input3 = analogRead(A0);

  PID1.Compute();
  PID2.Compute();
  //PID3.Compute();
}

void loop() {
  Input1 = analogRead(A1);
  Input2 = analogRead(A2);

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
  if (currentMillis - windowStartTime > WindowSize)
  {
    //time to shift the Relay Window
    //Serial.print("shift");
    windowStartTime += WindowSize;
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

  // if (Output3 >= currentMillis - windowStartTime) 
  // {
  //   digitalWrite(relay3, HIGH);
  //   //Serial.println("high");
  // }
  // else 
  // {
  //   digitalWrite(relay3, LOW);
  //   //Serial.println("low");
  // }

  // Get the data for time, output, relay state and input
  if ((currentMillis - lastSaveTime) >= 1000) 
  {
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
    //Serial.print("  ");

    // Serial.print(Output3);
    // Serial.print("  ");
    // Serial.print(digitalRead(relay3));
    // Serial.print("  ");
    // Serial.println(Input3);
    lastSaveTime = currentMillis;
  }
}

// // Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
// double GetTemperature()
// {
//   int val[3] = {analogRead(tempPin1), analogRead(tempPin1), analogRead(tempPin1)};
//   float voltage = 0;

//   double temp[3] = {};
//   for (int i = 0; i < 3; i++)
//   {
//     voltage = val[i] * (5 / 1023.0);
//     temp[i] = (voltage - 1.21) / 0.005;
//   }
//   if(CompareSensorReading(temp[0], temp[1], temp[2]) == true)
//   {
//     double tempAvg = (temp[0] + temp[1] + temp[2]) / 3;
//     tempAvg = round(tempAvg * 100.0) / 100.0;
//     return tempAvg;
//   }
//   else
//   {
//     // TODO: stop the heater and check the connection of temperature sensor
//     heaterStop = true;
//   }
//   return 0;
// }

// // Making the sure the 3 temperature sensor have similar readings. The difference  needs to be within 5 degree celcius
// bool CompareSensorReading(float temp1, float temp2, float temp3)
// {
//   if((abs(temp1 - temp2) && abs(temp1 - temp3) && abs(temp2 - temp3)) <= 5)
//   {
//     return true;
//   }
//   return false;
// }

// void DisplayHeating()
// {
//   unsigned long currentTime = millis();
//   float temp = GetTemperature();
//   if((currentTime - lastRefresh) >= refresehInterval) // Re-print the temperature reading on the lcd every 1s
//   {
//       lcd.setCursor(0,0);
//       lcd.print("SP = ");
//       lcd.setCursor(0,1);
//       lcd.print("Preheating: ");
//       lcd.setCursor(12, 1);
//       lcd.print(temp);
//       lcd.setCursor(9,0);
//       lcd.print(String(char(223)) + "C");
//       lcd.setCursor(5,0);
//       lastRefresh = currentTime;
//     }
// }