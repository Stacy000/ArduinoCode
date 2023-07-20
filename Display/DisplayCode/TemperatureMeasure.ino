int tempPin1=A0;
int tempPin2=A1;
int tempPin3=A2;

int val = 0;

// Get the readings from the thermocouple, and convert the readings to voltage first, then temperature in degree celsius 
float GetTemperature()
{
   val = analogRead(tempPin1);  
   float voltage= val * (5.0 / 1023.0);   
   float temp = (voltage - 1.25) / 0.005;
   return temp;
}