extern int tempPin1;
extern int tempPin2;
extern int tempPin3;

extern int val;

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
float GetTemperature()
{
   val = analogRead(tempPin1);  
   float voltage= val * (5.0 / 1023.0);   
   float temp = (voltage - 1.25) / 0.005;
   return temp;
}