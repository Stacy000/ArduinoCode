bool spooling = false;

volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;

void SetUpLimitSwitch()
{
  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS, INPUT_PULLUP);
}

void MotorHoming()
{
    spoolStepper.setAcceleration(1200);
    spoolStepper.moveTo(-5000);
    spoolStepper.runToPosition();
}
void PrepareMotor()
{
  if(motorBackward == true)
  {
    spoolStepper.setCurrentPosition(0);
    spoolStepper.setAcceleration(70);
    spoolStepper.moveTo(1300);
    spoolStepper.runToPosition();
    //motorBackward = false;
    //spooling = true;
  }
}

void RunSpoolMotor() {
  
  //Serial.print("Motor running\n");
  spoolStepper.setCurrentPosition(0);
  spoolStepper.setMaxSpeed(20);
  spoolStepper.setAcceleration(1000);
  spoolStepper.moveTo(1000);
  //spoolStepper.setSpeed(50);
  spoolStepper.runToPosition();

  spoolStepper.moveTo(0);
  spoolStepper.runToPosition();

  // if(spooling == true)
  // {
    
  // }
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