#include <SPI.h>

#include <stdio.h>

// Define the pins
int pulseNeg = 7;
int directionNeg = 8;
//int enableNeg = 10;

// Define variables
int pulsePerRev=200;
int rpm=30;

// Get pulse period
int getSleepTime(int pulsePerRev, int rpm) {

  int pulsePerSec=rpm/60*pulsePerRev;
  int sleepTime=1/pulsePerSec;

  return sleepTime;
}
int sleepTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pulseNeg, OUTPUT);
  pinMode(directionNeg, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
 //delayMicroseconds(100);
  digitalWrite(directionNeg, HIGH);
  digitalWrite(pulseNeg, HIGH);
  delayMicroseconds(200);
  digitalWrite(pulseNeg, LOW);
  //delayMicroseconds(2800);
  Serial.println(pulseNeg);
  delayMicroseconds(200);
  
}
