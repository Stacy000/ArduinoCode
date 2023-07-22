#include <AccelStepper.h>
#include <MultiStepper.h>

#define LS1 15
#define LS2 18

const int stepsPerRevolution = 10;

// Define a 4 wire stepper motor to pin 8 9 10 11
AccelStepper spoolStepper(4,8,9,10,11);

// Define variables
bool motorBackward = false;
bool motorHoming = true;
bool spooling = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;


void setup() {
  // Set one of the limit switch signal to high
  pinMode(LS1, OUTPUT);
  digitalWrite(LS1, HIGH);

  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS2, INPUT_PULLUP);
  
  // An interrupt that triggers when the limit switch is pressed
  attachInterrupt(digitalPinToInterrupt(LS2), stopMotor, FALLING);

  // Set the maximum speed to 150
  spoolStepper.setMaxSpeed(150);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);
}


void loop() {
  
  if(motorHoming == true)
  {  
    spoolStepper.setAcceleration(1200);
    spoolStepper.moveTo(-5000);
    spoolStepper.runToPosition();
  }

  if(motorBackward == true)
  {
    spoolStepper.setCurrentPosition(0);
    spoolStepper.setAcceleration(70);
    spoolStepper.moveTo(1300);
    spoolStepper.runToPosition();
    motorBackward = false;
    spooling = true;
  }

  if(spooling == true)
  {
    spoolStepper.setCurrentPosition(0);
    spoolStepper.setMaxSpeed(20);
    spoolStepper.setAcceleration(1000);
    spoolStepper.moveTo(1000);
    //spoolStepper.setSpeed(50);
    spoolStepper.runToPosition();

    spoolStepper.moveTo(0);
    spoolStepper.runToPosition();
  }
}

void stopMotor(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime >= interruptInterval) {
    spoolStepper.stop();
    //Serial.print("Switch pressed");
    motorBackward = true;
    motorHoming = false;
    //Serial.write("\n");
    // Update the last interrupt time
    lastInterruptTime = currentMillis;
  }
}