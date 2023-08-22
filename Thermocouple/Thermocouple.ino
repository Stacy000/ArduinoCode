int tempPin1=A0;
int tempPin2=A1;
int tempPin3=A2;

int val = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A2));
   val = analogRead(A0);  
   float voltage= val * (5 / 1023.0);   
   //Serial.println(voltage);
   float temp=(voltage - 1.21) / 0.005;
   //Serial.println(temp);
   delay(500);
}