extern int DC1;
extern int DC2;
extern int DC_EnB;

extern int LS;
extern AccelStepper spoolStepper;

extern bool motorBackward;
extern bool motorHoming;
extern bool spoolForward;
extern bool spoolBackward;

volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;

void Homing() {
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(-5000);
  spoolStepper.run();
}

void Prepare() {
  spoolStepper.setMaxSpeed(150);
  spoolStepper.setAcceleration(500);
  spoolStepper.moveTo(1300);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 1300) {
    motorBackward = false;
    spoolForward = true;
    ClearLCD();
    state = 0;
  }
}

void SpoolingFoward() {
  spoolStepper.setMaxSpeed(50);
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(2300);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 2300) {
    spoolForward = false;
    spoolBackward = true;
    //spoolStepper.stop();
  }
}

void SpoolingBackward() {
  spoolStepper.setMaxSpeed(50);
  spoolStepper.setAcceleration(800);
  spoolStepper.moveTo(1300);
  spoolStepper.run();

  if (spoolStepper.currentPosition() == 1300) {
    spoolBackward = false;
    spoolForward = true;
    //spoolStepper.stop();
  }
}

void RunDCMotor() {
  digitalWrite(DC1, LOW);
  digitalWrite(DC2, HIGH);
  analogWrite(DC_EnB, 180);
}

void stopMotor() {
  unsigned long currentMillis = millis();

  // TODO: Avoid overflow risk
  
  if ((currentMillis - lastInterruptTime) >= interruptInterval) {
    spoolStepper.stop();
    Serial.print("Switch pressed");
    Serial.write("\n");
    // Update the last interrupt time
    lastInterruptTime = currentMillis;
    spoolStepper.setCurrentPosition(0);
    motorBackward = true;
    motorHoming = false;
  }
}