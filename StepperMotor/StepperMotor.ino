#include <SPI.h>

#include <stdio.h>

// Define the pins
int pulseNeg = 4;
int directionNeg = 5;
int enableNeg = 7;

bool pulseActive = false;
unsigned long previousTime = 0;

// Define variables
int pulsePerRev=200;
int rpm=30;

// Get pulse period
float getSleepTime() {

  float pulsePerSec=rpm/60*pulsePerRev;
  //Serial.print("hi");
  float sleepTime = 1000/pulsePerSec;
  //Serial.print("why");
  return sleepTime;
}
float sleepTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pulseNeg, OUTPUT);
  pinMode(directionNeg, OUTPUT);
  pinMode(enableNeg, OUTPUT);

  digitalWrite(enableNeg, LOW);
}

void loop() {
  //runMotor();
  // put your main code here, to run repeatedly:
  sleepTime = getSleepTime();
  digitalWrite(enableNeg, HIGH);
 //delayMicroseconds(100);
  digitalWrite(directionNeg, HIGH);
  digitalWrite(pulseNeg, HIGH);
  delayMicroseconds(sleepTime);
  digitalWrite(pulseNeg, LOW);
  //delayMicroseconds(2800);
  Serial.println(pulseNeg);
  delayMicroseconds(sleepTime);
  
}

void runMotor()
{
  digitalWrite(directionNeg, LOW);
  digitalWrite(enableNeg, HIGH);

  unsigned long currentTime = millis();

  if (digitalRead(pulseNeg) == HIGH && currentTime - previousTime >= 10) {
    digitalWrite(pulseNeg, LOW);
    //Serial.println("low");
  }

  if (currentTime - previousTime >= 20) {
    // Generate a pulse
    digitalWrite(pulseNeg, HIGH);
    //Serial.println("high");
    previousTime = currentTime;
  }

}
