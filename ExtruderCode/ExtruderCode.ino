#include "Encoder.h"
#include "DisplayTime.h"
#include "SpoolStepper.h"
#include "TemperatureReading.h"
#include "MainStepper.h"

LiquidCrystal_I2C lcd(0x27,20,4);

// Define pin for DC motor
#define DC1 36
#define DC2 38
#define DC_EnB 6
bool dcDecreasing = true;

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
// Define unused pins
int unusedPins[]= {0,1,4,5,7,12,13,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,14,15,16,17,18,22,23,24,25,
                    26,27,28,29,30 ,32,33,34,35,37,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};

// Define variables
volatile unsigned long lastRefresh = 0;
const unsigned long refresehInterval = 1000;

void setup() 
{
  Serial.begin(4800);
  // put your setup code here, to run once:
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);

  // Set the DC motor
  pinMode(DC1, OUTPUT);
  pinMode(DC2, OUTPUT);
  digitalWrite(DC1, LOW);
  digitalWrite(DC2, HIGH);

  // Set the other limit switch signal as input; the pin is at high state by default
  pinMode(LS, INPUT_PULLUP);

  // Set the main stepper motor
  pinMode(pulseNeg, OUTPUT);
  pinMode(directionNeg, OUTPUT);

  int size= sizeof(unusedPins)/sizeof(int);

  // Set the unused pins to Output LOW signal
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
  
  // int stat = digitalRead(LS);
  // Serial.println(stat);

  attachInterrupt(digitalPinToInterrupt(LS), stopMotor, FALLING); //Note: The interrupt function triggers once when initializing the attachInterrupt. The signal of the LS pin is HIGH before and after this line.

  // stat = digitalRead(LS);
  // Serial.println(stat);

  //Set up the lcd
  lcd.init();
  lcd.backlight();

  // Display the welcome page
  lcd.setCursor(0,0); 
  lcd.print("WELCOME");
  lcd.setCursor(0,1); 
  lcd.print("Test Version"); 
  delay(2000); //wait 2 sec

  // Clear the LED for next page
  lcd.clear();

  // Set the maximum speed to 150
  spoolStepper.setMaxSpeed(200);

  // Set the current motor position to 0
  spoolStepper.setCurrentPosition(0);
  lcd.setCursor(0, 0);
  lcd.print("Homing");
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
    DisplayHeating();
  }

  // State stays at 3 when the main motor is running
  if(state == 3)
  {
    lcd.setCursor(0,0); 
    lcd.print("Spooling...");
    DisplayTime();
    lcd.setCursor(0,1);
    lcd.print("Start motor");
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

      Serial.println(dcSpeed);
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
  Serial.println("?");
  lcd.setCursor(1,0);
  lcd.print("YOU HAVE SELECTED");
  Serial.println("!");
  int i = CheckCurrentSelection();
  lcd.setCursor(1,1);

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
  float temp = GetTemperature();

  if((currentTime - lastRefresh) >= refresehInterval) // Re-print the temperature reading on the lcd every 1s
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

  // Finish the heating process when the temperature is at the set point
  if(temp >= 900)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heating is done");
    state = 3;
    lcd.clear();
    spoolingTimer.start();
  }
}
