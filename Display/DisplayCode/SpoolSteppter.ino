
// Define variables
bool motorBackward = false;
bool motorHoming = true;
bool spooling = false;

volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;

void SetUpLimitSwitch()
{
  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS, INPUT_PULLUP);
}

void RunSpoolMotor() {
  
  Serial.print("Motor running\n");
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
    Serial.print("Switch pressed");
    motorBackward = true;
    motorHoming = false;
    //Serial.write("\n");
    // Update the last interrupt time
    lastInterruptTime = currentMillis;
  }
}