#include "Encoder.h"
#include "DisplayTime.h"
#include "SpoolStepper.h"
#include "TemperatureReading.h"

LiquidCrystal_I2C lcd(0x27,20,4);

// Define pin for DC motor
#define DC1 36
#define DC2 38
#define DC_EnB 6
bool dcDecreasing = true;

// Get the user defined type variables across files
extern Timer timer;
extern AccelStepper spoolStepper;

// Get the lcd variables
extern bool refreshLCD;
extern bool refreshSelection;
extern bool clearSelection;
extern int state;

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
  attachInterrupt(digitalPinToInterrupt(encoderCLK), rotate, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderSW), pushButton, FALLING);
  
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

  // Display the user select material
  if(state == 1)
  {
    lcd.clear();
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
    lcd.clear();
  }

  // Display the temperature and the heating time on lcd
  if(state == 2)
  {
    unsigned long currentTime = millis();
    float temp = GetTemperature();

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
    if(temp >= 50)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Heating is done");
      state = 3;
     lcd.clear();
    }
  }

  if(state==3)
  {
    lcd.setCursor(0,0);
    lcd.print("Spooling");
    lcd.setCursor(1, 0);
    

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


