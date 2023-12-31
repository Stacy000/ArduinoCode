#include "SpoolStepper.h"
#include "Encoder.h"

#include <Arduino.h>

// Define a 4 wire stepper motor to pin 8 9 10 11
AccelStepper spoolStepper(4,8,9,10,11);
extern LiquidCrystal_I2C lcd;

// Define variables for spooling stepper motor
bool motorBackward = false;
bool motorHoming = true;

bool spoolForward = false;
bool spoolBackward = false;

int state = 999;

bool startMotor = false;

volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;

void Homing() {
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(-5000);
  spoolStepper.run();
}

void Prepare() {
  spoolStepper.setMaxSpeed(100);
  spoolStepper.setAcceleration(500);
  spoolStepper.moveTo(1200);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 1200) {
    motorBackward = false;
    spoolForward = true;
    lcd.clear();
    state = 0;
    spoolStepper.stop();
  }
}

void SpoolingFoward() {
  spoolStepper.setMaxSpeed(50);
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(2100);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 2100) {
    spoolForward = false;
    spoolBackward = true;
    //spoolStepper.stop();
  }
}

void SpoolingBackward() {
  spoolStepper.setMaxSpeed(50);
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(1200);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 1200) {
    spoolBackward = false;
    spoolForward = true;
    //spoolStepper.stop();
  }
}

void stopMotor() {
  unsigned long currentMillis = millis();

  // TODO: Avoid overflow risk
  
  if ((currentMillis - lastInterruptTime) >= interruptInterval) 
  {
    // Homing only happens at the start of the program (state 999), avoid unexpected limit switch trigerring at other stages
    if(state==999)
    {
      spoolStepper.stop();
      //Serial.println("Switch pressed");
      // Update the last interrupt time
      lastInterruptTime = currentMillis;
      spoolStepper.setCurrentPosition(0);
      motorBackward = true;
      motorHoming = false;
    }

    if(state == 3)
    {
      startMotor = true;
    }
  }
}