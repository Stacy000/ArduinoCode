#include <AccelStepper.h>
#include <MultiStepper.h>

// Define timer
Timer timer;

// Define pin for limit switch
int LS = 19; //Interrupt pin

// Define pin for DC motor
int DC1 = 36;
int DC2 = 38;
int DC_EnB = 6;

// Define pins for rotary encoder for the menu
int encoderCLK = 3; //Interrupt pin
int encoderDT = 31;
int encoderSW = 2; //Interrupt pin

// Define a 4 wire stepper motor to pin 8 9 10 11
AccelStepper spoolStepper(4,8,9,10,11);

// Define unused pins
int unusedPins[]= {0,1,4,5,7,12,13,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,14,15,16,17,18,22,23,24,25,
                    26,27,28,29,30,31,32,33,34,35,37,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};

// Define variables for spooling stepper motor
bool motorBackward = false;
bool motorHoming = true;

bool spoolForward = false;
bool spoolBackward = false;

volatile unsigned long lastRefresh = 0;
const unsigned long refresehInterval = 1000;

bool refreshLCD = true;
bool refreshSelection = false;
bool clearSelection = false;

//Define pins and variables for temperature reading
int tempPin1 = A0;
int tempPin2 = A1;
int tempPin3 = A2;
int val = 0;

int state = 999;

void setup() 
{
  Serial.begin(4800);
  // put your setup code here, to run once:
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);
  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS, INPUT_PULLUP);

  int size= sizeof(unusedPins)/sizeof(int);
  // Set the unused pins to Output 0
  for(int i = 0; i < size; i++)
  {
    pinMode(unusedPins[i], OUTPUT);
    digitalWrite(unusedPins[i], LOW);
  } 

  SetUpLCD();
  
  // Define the interrupt functions
  attachInterrupt(digitalPinToInterrupt(encoderCLK), rotate, FALLING);
  
  attachInterrupt(digitalPinToInterrupt(encoderSW), pushButton, FALLING);
  int stat = digitalRead(LS);

  Serial.println(stat);
  attachInterrupt(digitalPinToInterrupt(LS), stopMotor, FALLING);
  stat = digitalRead(LS);

  Serial.println(stat);

  // Set the maximum speed to 150
  spoolStepper.setMaxSpeed(200);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);
  lcd.setCursor(0, 0);
  lcd.print("Homing");

  // Set the initial state to 0
  state = 999;

  // Delay for stablization
  
  delay(2000);
}

void loop() {

  // if(state == 999)
  // {
  //   if(motorHoming == true)
  //   {  
  //     Homing();
  //     //Serial.print("Homing");
  //   }

  //   if(motorBackward == true)
  //   {
  //     Prepare();
  //   }
  // }

  //Put your main code here, to run repeatedly:
  

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
    
    RunDCMotor(); 
    if(spoolForward == true)
    {
      SpoolingFoward();
    }

    if(spoolBackward == true) 
    {
      SpoolingBackward();
    }
  }
}
