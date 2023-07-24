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
bool spoolForward = false;
bool spoolBackward = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;


void setup() {

  Serial.begin(4800);
  // Set one of the limit switch signal to high
  pinMode(LS1, OUTPUT);
  digitalWrite(LS1, HIGH);

  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS2, INPUT_PULLUP);
  
  // An interrupt that triggers when the limit switch is pressed
  attachInterrupt(digitalPinToInterrupt(LS2), stopMotor, FALLING);

  // Set the maximum speed to 150
  spoolStepper.setMaxSpeed(200);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);
}


void loop() {

  if(motorHoming == true)
  {  
    Homing();
  }

  if(motorBackward == true)
  {
    Prepare();
  }

  if(spoolForward == true)
  {
    SpoolingFoward();
  }

  if(spoolBackward == true) 
  {
    SpoolingBackward();
  }
}

void stopMotor(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime >= interruptInterval) {
    spoolStepper.stop();
    //Serial.print("Switch pressed");
    //Serial.write("\n");
    // Update the last interrupt time
    lastInterruptTime = currentMillis;
    spoolStepper.setCurrentPosition(0);
    motorBackward = true;
    motorHoming = false;
  }
}

void Homing()
{
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(-5000);
  spoolStepper.run();
}

void Prepare()
{
  spoolStepper.setMaxSpeed(150);
  spoolStepper.setAcceleration(500);
  spoolStepper.moveTo(1300);
  spoolStepper.run();
  
  if(spoolStepper.currentPosition()==1300)
  {
    motorBackward = false;
    spoolForward = true;
  }
}

void SpoolingFoward()
{
  spoolStepper.setMaxSpeed(20);
  spoolStepper.setAcceleration(1000);
  spoolStepper.moveTo(2300);
  spoolStepper.run();

  if(spoolStepper.currentPosition() == 2300)
  {
    spoolForward = false;
    spoolBackward = true;
    //spoolStepper.stop();
  }
}

void SpoolingBackward()
{
  spoolStepper.setMaxSpeed(20);
  spoolStepper.setAcceleration(1000);
  spoolStepper.moveTo(1300);
  spoolStepper.run();

  if(spoolStepper.currentPosition() == 1300)
  {
    spoolBackward = false;
    spoolForward = true;
    //spoolStepper.stop();
  }
}