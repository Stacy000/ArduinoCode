#include <AccelStepper.h>
#include <MultiStepper.h>

Timer timer;

#define LS 18 //Interrupt pin

// Define a 4 wire stepper motor to pin 8 9 10 11
AccelStepper spoolStepper(4,8,9,10,11);

bool beginMotor = false;

// Define variables
bool motorBackward = false;
bool motorHoming = true;

volatile unsigned long lastRefresh = 0;
const unsigned long refresehInterval = 1000;

void setup() 
{
  Serial.begin(4800);
  // put your setup code here, to run once:
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);

  SetUpLCD();
  SetUpLimitSwitch();

  // Store states of the rotary encoder
  // CLKPrev = digitalRead(encoderCLK);
  // DTPrev = digitalRead(encoderDT);
  
  // Define the interrupt functions
  attachInterrupt(digitalPinToInterrupt(encoderCLK), rotate, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderSW), pushButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(LS), stopMotor, FALLING);

  // Set the maximum speed to 150
  spoolStepper.setMaxSpeed(150);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);

  // Set the initial state to 0
  state = 999;
}

void loop() {
  // Put your main code here, to run repeatedly:

  if(state = 999)
  {
    lcd.setCursor(0, 0);
    lcd.print("Preparing");
    if(motorHoming == true)
    {
      MotorHoming();
    }

    if(motorBackward==true)
    {
      // Send the motor to the initial position
      PrepareMotor();
      state = 0;

    }
  }


  if(state == 0)
  {
    DisplayMaterialSelection();

    // refreshLCD is true only when the interrupts are triggered
    if(refreshLCD == true) 
    {
      updateCursorPosition(); //update the position
      refreshLCD = false; //reset the variable - wait for a new trigger
    }

    // refreshSelection is only true when the pushbutton is pressed
    if(refreshSelection == true) //if the selection is changed
    {
      updateSelection();
      refreshSelection = false; // reset the variable - wait for a new trigger
    }
  }

  if(state == 1)
  {
    ClearLCD();
    lcd.setCursor(1,0);
    lcd.print("YOU HAVE SELECTED");
    int i = CheckCurrentSelection();
    lcd.setCursor(1,3);

    switch(i)
    {
      case 0:
      lcd.print("ABS");
      break;

      case 1: 
      lcd.print("PETG");
      break;

      case 2:
      lcd.print("PET");
      break;

      case 3:
      lcd.print("PETE");
      break;

      default:
      lcd.print("Nothing");
    }
    state = 2;
    timer.start();
    ClearLCD();
  }

  if(state == 2)
  {
    unsigned long currentTime = millis();
    float temp= GetTemperature();

    if((currentTime - lastRefresh) >= refresehInterval)
    {
      // Serial.print(currentTime);
      // Serial.print("\n");
      // Serial.print(lastRefresh);
      // Serial.print("\n");
      lcd.setCursor(0, 0);
      lcd.print("heating...");
      lcd.setCursor(0, 1);
      lcd.print(temp);
      DisplayTime();
      lastRefresh = currentTime;
    }

    if(temp > 24)
    {
      state = 3;
      ClearLCD();
    }
  }


  if(state == 3)
  {
    lcd.setCursor(0,0);
    lcd.print("Motor is running");    
    RunSpoolMotor();
  }
  
  if (beginMotor == true)
  {
    

    //delay(1000);
    
  }
}
