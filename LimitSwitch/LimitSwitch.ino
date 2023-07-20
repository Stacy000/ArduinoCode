#include <Stepper.h>
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA A8
#define ENB A9

#define SW1 15
#define SW2 18

const int stepsPerRevolution = 500;
Stepper myStepper = Stepper(stepsPerRevolution, 8,9,10,11);

bool motorStop = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long interruptInterval = 500;

void setup() {
  pinMode(IN1, OUTPUT);
  
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);

  pinMode(IN4, OUTPUT);
  
  pinMode(SW1, OUTPUT);
  pinMode(SW2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(SW2), stopMotor, FALLING);

  digitalWrite(SW1, HIGH);
  
  Serial.begin(4800);

  myStepper.setSpeed(10);
    
  motorReset();
}


void loop() {

// Step one revolution in one direction:
  myStepper.step(500);

  //delay(2000);

  // Step on revolution in the other direction:
  myStepper.step(-500);
  // digitalWrite(IN1, HIGH);
  // digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);

  // // analogWrite(ENA, 50);
  // // analogWrite(ENB, 50);


  // delay(1000);

  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, HIGH);
  // digitalWrite(IN3, HIGH);
  // digitalWrite(IN4, LOW);

  // // analogWrite(ENA, 50);
  // // analogWrite(ENB, 50);
  
  // // for(int i=0; i<1200; i++) {
  // //   digitalWrite(pul, HIGH);
  // //   delay(1);
  // //   digitalWrite(pul, LOW);
  // //   delay(1);
  // //   }
  // Serial.print("Nothing");
  // Serial.write("\n");
  // // delay(1000);
}

void stopMotor(){
  unsigned long currentMillis = millis();
  if (currentMillis - lastInterruptTime >= interruptInterval) {
    motorStop=true;
    Serial.print("Switch pressed");
    Serial.write("\n");
    // Update the last interrupt time
    lastInterruptTime = currentMillis;
  }
}

void motorReset(){
  // Serial.print("Start homing");
  // Serial.write("\n");
  
  
  
  // Serial.print("Homing");
  // Serial.write("\n");
  // digitalWrite(dir, LOW);// Set the homing direction
  // // Keep rotating till the switch is pressed
  // while(motorStop==false){
  //   digitalWrite(pul, HIGH);
  //   delay(1);
  //   digitalWrite(pul, LOW);
  //   delay(1);
  // }

  // Serial.print("Adjusting position");
  // Serial.write("\n");
  // digitalWrite(dir, HIGH);
  // for(int i=0; i<1000; i++) {
  //   digitalWrite(pul, HIGH);
  //   delay(1);
  //   digitalWrite(pul, LOW);
  //   delay(1);

  //   }
  // Serial.print("Homing finished");
  // Serial.write("\n");
}