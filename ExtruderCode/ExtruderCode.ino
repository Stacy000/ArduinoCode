#include "Encoder.h"
#include "DisplayTime.h"
#include "SpoolStepper.h"
#include "TemperatureReading.h"
#include "MainStepper.h"
#include "Fan.h"
#include "Heater.h"
#include "HallEffectSensor.h"

LiquidCrystal_I2C lcd(0x27,20,4);

// Define pin for DC motor
#define DC1 36
#define DC2 38
#define DC_EnB 6

// Define pin for LEDs
#define red 44
#define yellow 46
#define green 48

bool dcDecreasing = true;
int currentSelection = -1;
double preHeatingTemp = 0;

#define cancelButton 18
bool cancelSystem = false;

// Get the user defined type variables across files
extern Timer heatingTimer;
extern Timer spoolingTimer;
extern AccelStepper spoolStepper;

// Get the lcd variables
extern bool refreshLCD;
extern bool refreshSelection;
extern bool clearSelection;
extern int state;
extern bool selectYes;
extern bool selectBack;
extern bool selectNext;

// Get the spool motor variables
extern bool motorBackward;
extern bool motorHoming;
extern bool spoolForward;
extern bool spoolBackward;
extern bool startMotor;

// Get the temperature variables
extern int setPointABS;
extern int setPointPETG;
extern int setPointPLA;
extern int setPointPETE;

// Get the heater variables
extern bool steadyState;

extern int rpm;
extern int pulsePerRev;
extern unsigned long previousTime;

// Define unused pins
int unusedPins[]= {0,1,4,5,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,14,15,16,17,18,22,23,24,25,
                    26,27,28,29,30,32,33,34,35,37,39,40,41,42,52,53};

// Define variables
volatile unsigned long lastRefresh = 0;
const unsigned long refresehInterval = 1000;
double sleepTime;
bool startPID = false;

void setup() 
{
  Serial.begin(4800);

  // Set the reference voltage of analog pin to 2.56V
  analogReference(INTERNAL2V56);

  // Set the rotary encoder
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);

  // Set the DC motor
  pinMode(DC1, OUTPUT);
  pinMode(DC2, OUTPUT);
  digitalWrite(DC1, LOW);
  digitalWrite(DC2, HIGH);

  // Set the four fans
  pinMode(fanIn1, OUTPUT);
  pinMode(fanIn2, OUTPUT);
  pinMode(fanIn3, OUTPUT);
  pinMode(fanIn4, OUTPUT);

  // Set the three pins for LEDs
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // Set the relay
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  
  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS, INPUT_PULLUP);

  // Set the main stepper motor
  pinMode(pulseNeg, OUTPUT);
  pinMode(directionNeg, OUTPUT);
  pinMode(enableNeg, OUTPUT);

  // Set the hall effect sensor
  pinMode(LED_hallEffect, OUTPUT);
  pinMode(hallEffectSensor, INPUT);

  // Disable the main motor to avoid interference
  digitalWrite(enableNeg, LOW);

  // Set the unused pins to Output LOW signal
  int size= sizeof(unusedPins)/sizeof(int);
  for(int i = 0; i < size; i++)
  {
    pinMode(unusedPins[i], OUTPUT);
    digitalWrite(unusedPins[i], LOW);
  } 

  // Delay for stablization
  delay(200);

  // Define the interrupt functions
  attachInterrupt(digitalPinToInterrupt(encoderCLK), Rotate, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderSW), PushButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(cancelButton), CancelSystem, HIGH);
  
  // int stat = digitalRead(LS);
  // Serial.println(stat);

  attachInterrupt(digitalPinToInterrupt(LS), stopMotor, FALLING); //Note: The interrupt function triggers once when initializing the attachInterrupt. The signal of the LS pin is HIGH before and after this line.

  // stat = digitalRead(LS);
  // Serial.println(stat);

  // Setup the parameters for PID controller
  SetUpPID();
  //Set up the lcd
  lcd.init();
  lcd.backlight();

  // Display the welcome page
  lcd.setCursor(0,0); 
  lcd.print("WELCOME");
  lcd.setCursor(0,1); 
  lcd.print("Test Version"); 
  delay(5000); //wait 2 sec

  // Clear the LED for next page
  lcd.clear();

  // Set the maximum speed to 100
  spoolStepper.setMaxSpeed(100);

  sleepTime = GetSleepTime();
  //Serial.print(sleepTime);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);
  lcd.setCursor(0, 0);
  lcd.print("Homing");
  digitalWrite(yellow, HIGH);
}

void loop() {

  // Bring the motor to a fixed initial position
  if(state == 999)
  {
    if(motorHoming == true)
    {  
      Homing();
      //Serial.print("Homing");
    }

    if(motorBackward == true)
    {
      Prepare();
    }
  }  

  // Allow the user to select the material 
  if(state == 0)
  {
    // halt the stepper motor to avoid overheating
    StepperIdle();

    // Display the menu for the user to select material
    DisplayMaterialSelection();

    // refreshLCD is true only when the interrupts are triggered
    if(refreshLCD == true) 
    {
      UpdateCursorPosition(); //update the position
      refreshLCD = false; //reset the variable - wait for a new trigger
    }

    // refreshSelection is only true when the pushbutton is pressed
    if(refreshSelection == true) //if the selection is changed
    {
      UpdateSelection();
      refreshSelection = false; // reset the variable - wait for a new trigger
    }
  }

  // Display the user select material
  if(state == 1)
  {
    // halt the stepper motor to avoid overheating
    StepperIdle();
    if(selectNext == true)
    {
      lcd.clear();
      selectNext = false;
    }
    DisplayUserSelection();
  }

  // Display the temperature and the heating time on lcd
  // State becomes 3 when the heating process is done
  if(state == 2)
  {
    // Start to heat up the extruder
    startPID = true;

    // halt the stepper motor to avoid overheating
    StepperIdle(); 

    // Display temperature on the lcd durig the heating process
    DisplayHeating();

    // The extruder starts to spool filament when the temperature inside the extruder reaches steady state
    if(steadyState == true)
    {
      state = 3;
      lcd.clear();
      spoolingTimer.start();
    }
  }

  // Start heating up the extruder to the set point temperature using PI controller
  if(startPID == true)
  {
    switch(currentSelection)
    {
      case 0:
      RunPID(setPointABS);
      break;
      
      case 1:
      RunPID(setPointPETG);
      break;

      case 2:
      RunPID(setPointPLA);
      break;

      case 3:
      RunPID(setPointPETE);
      break;
    }
  }

  // State stays at 3 when the main motor is running
  if(state == 3)
  {
    RunMainStepper(sleepTime);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
    lcd.setCursor(0,0); 
    lcd.print("Spooling...");
    DisplayTime();
    lcd.setCursor(0,1);
    lcd.print("Start motor");

    // TODO: display the diameter on the lcd

    TurnOnFan();
  }
 
  if(startMotor == true)
  {
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

  if(cancelSystem == true)
  {
    digitalWrite(green,LOW);
    digitalWrite(red, HIGH);
    if(state == -1)
    {
      lcd.clear();
      state = -2;
    }
    lcd.setCursor(0, 0);
    lcd.print("Cancelling");
    lcd.setCursor(0, 1);
    lcd.print("Do not leave until");
    lcd.setCursor(0, 2);
    lcd.print("Cools down");

    spoolStepper.stop();

    // Set all the pins of the motor to LOW to avoid overheating the L298N module
    StepperIdle();
    analogWrite(DC_EnB, 0);

    // Start cooling down the temperature
    // Stop the main stepper motor when nothing is spooling out of the extruder

    TurnOffFan();
  }
}

// Function that controls the speed of the dc motor
void RunDCMotor() 
{
  int dcSpeed = 256;

  // To overcome the holding torque
  if(dcDecreasing == true)
  {
    for (dcSpeed = 256; dcSpeed >= 70; dcSpeed--)
    {
      analogWrite(DC_EnB, dcSpeed);
      if(dcSpeed == 80)
      {
        dcDecreasing = false;
        break;
      }
    }
  }

  else
  {
    analogWrite(DC_EnB, 100);
  }
}

// Function is called when the state is 1
// If the user select Yes, the state jumps to 2; if the user select Back, the state jumps to 0.
void DisplayUserSelection()
{
  lcd.setCursor(1,0);
  lcd.print("YOU HAVE SELECTED");
  currentSelection = CheckCurrentSelection();
  lcd.setCursor(1,1);

  switch(currentSelection)
  {
    case 0:
    lcd.print("ABS");
    break;

    case 1: 
    lcd.print("PETG");
    break;

    case 2:
    lcd.print("PLA");
    break;

    case 3:
    lcd.print("PETE");
    break;

    default:
    lcd.print("Nothing");
  }

  lcd.setCursor(4,2);
  lcd.print("Countinue?");
  lcd.setCursor(4,3);
  lcd.print("Yes");
  lcd.setCursor(9,3);
  lcd.print("Back");

  if(refreshLCD == true)
  {
    UpdateStateOneCursor();
    refreshLCD = false; 
  }

  if(selectYes == true)
  {
    state = 2;
    heatingTimer.start();
    lcd.clear();
    selectYes = false;
  }

  if(selectBack == true)
  {
    state = 0;
    ResetAllSelection();
    lcd.clear();
    selectBack = false;
  }
}

// Function that displays the current temperature and time that has passed since the beginning of the heating process.
void DisplayHeating()
{
  unsigned long currentTime = millis();

  double temp = GetTemperature();

  if((currentTime - lastRefresh) >= refresehInterval) // Re-print the temperature reading on the lcd every 1s
  {
      lcd.setCursor(0,0);
      lcd.print("SP = ");
      lcd.setCursor(0,1);
      lcd.print("Preheating: ");
      lcd.setCursor(12, 1);
      lcd.print(temp);
      lcd.setCursor(9,0);
      lcd.print(String(char(223)) + "C");
      lcd.setCursor(5,0);

      switch(currentSelection)
      {
        case 0:
        lcd.print(setPointABS);
        //preHeatingTemp = setPointABS - 10;
        break;
        
        case 1:
        lcd.print(setPointPETG);
        //preHeatingTemp = setPointPETG - 10;
        break;

        case 2:
        lcd.print(setPointPLA);
        //preHeatingTemp = setPointPLA - 10;
        break;

        case 3:
        lcd.print(setPointPETE);
        //preHeatingTemp = setPointPETE - 10;
        break;

        default: 
        //Serial.println(currentSelection);
        break;
      }

      DisplayTime();
      lastRefresh = currentTime;
    }
}

void CancelSystem()
{
  state = -1;
  cancelSystem = true;
  startMotor = false;
}

// Rest the stepper motor driver
void StepperIdle()
{
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}