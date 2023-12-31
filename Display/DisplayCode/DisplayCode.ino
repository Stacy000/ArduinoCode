#include <Timer.h>

// Libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);

extern int state;

extern int encoderCLK; //Interrupt pin
extern int encoderDT;
extern int encoderSW; //Interrupt pin

// Define variables for material selection state (state 0)
int menuCounter = 0;
bool selectABS = false;
bool selectPETG = false;
bool selectPET = false;
bool selectPETE =false;
bool selectNext = false;

extern bool refreshLCD;
extern bool refreshSelection;
extern bool clearSelection;

void SetUpLCD()
{
  lcd.init();
  lcd.backlight();

  // Display the welcome page
  lcd.setCursor(0,0); 
  lcd.print("WELCOME");
  lcd.setCursor(0,1); 
  lcd.print("Test Version"); 
  delay(2000); //wait 2 sec

  // Clear the LED for next page
  ClearLCD();
}

// Clear the lcd screen
void ClearLCD()
{
  lcd.clear();
}

// Display the material selection page
void DisplayMaterialSelection()
{
  lcd.setCursor(1,0);
  lcd.print("ABS");

  lcd.setCursor(1,1);
  lcd.print("PETG");

  lcd.setCursor(1,2);
  lcd.print("PET");

  lcd.setCursor(1,3);
  lcd.print("PETE");

  lcd.setCursor(14,3);
  lcd.print("Next");
  lcd.setCursor(18,3);
  lcd.print("=");
  lcd.setCursor(19,3);
  lcd.print(">");
}

// Function to clear the all cursors except for the "X" on the selected material
void ClearCurrentCursor(int num)
{
  lcd.setCursor(0,0); 
  lcd.print(" "); 
  lcd.setCursor(0,1);
  lcd.print(" "); 
  lcd.setCursor(0,2);
  lcd.print(" "); 
  lcd.setCursor(0,3);
  lcd.print(" "); 
  lcd.setCursor(13,3);
  lcd.print(" ");

 // Keep the X for the material that has been selected
  if(num<5)
  {
    lcd.setCursor(0,num);
    lcd.print("X");
  }
}

// Check which material is selected
// Return 0 for ABS; 1 for PETG; 2 for PET; 3 for PETE; 5 if nothing has been selected
int CheckCurrentSelection()
{
  if(selectABS==true)
  {
    return 0;
  }

  if(selectPETG==true)
  {
    return 1;
  }

  if(selectPET==true)
  {
    return 2;
  }

  if(selectPETE==true)
  {
    return 3;
  }

  return 5;
}

// Move the ">" cursor
void updateCursorPosition()
{
  int keep = CheckCurrentSelection();

  ClearCurrentCursor(keep);
  
  // When the user is in the material selection state
  if(state == 0)
  {
    // Place cursor to the new position
    switch(menuCounter) 
    {
      case 0:
      lcd.setCursor(0,0); //1st line, 1st block
      lcd.print(">"); 
      break;
      //-------------------------------
      case 1:
      lcd.setCursor(0,1); //2nd line, 1st block
      lcd.print(">"); 
      break;
      //-------------------------------    
      case 2:
      lcd.setCursor(0,2); //3rd line, 1st block
      lcd.print(">"); 
      break;
      //-------------------------------    
      case 3:
      lcd.setCursor(0,3); //4th line, 1st block
      lcd.print(">"); 
      break;
      
      case 4: 
      lcd.setCursor(13,3); //4th line, 13th block
      lcd.print(">");
      break;
    }
  }
}

// This function is triggered when the CLK signal from the encoder is at its falling edge
void rotate()
{
  //Serial.print("FALLING");
  //Serial.print("\n");
  int i = digitalRead(encoderDT);

  // Serial.print(i);
  // Serial.print("\n");

  switch(i)
  {
    // COUNTER CLOCKWISE
    case 0: 
    menuCounter--;
    if(menuCounter < 0)
    {
      menuCounter = 4;
    }     
    break;
    
    // CLOCKWISE
    case 1: 
    menuCounter++;
    if(menuCounter > 4)
    {
      menuCounter = 0;
    }
    break;
  }

  //Refresh LCD after changing the counter's value
  refreshLCD = true;
  //clearSelection=true;
}

// Update the selection marker everytime the push button is pressed
void pushButton()
{
  //Serial.print("Button pressed!");  

  if(menuCounter==4)
  {
    selectNext =true;
    state = 1;
  }

  if(state==0)
  {
    // If any of the material has been selected
    if(selectABS==true || selectPETG==true || selectPETE==true ||selectPET==true)
    {
      // Clear the selection
      selectABS=selectPETE=selectPET=selectPETG = false;
    }

    switch(menuCounter)
    {
      case 0:
      selectABS = true;
      break;

      case 1:
      selectPETG = true; 
      break;

      case 2: 
      selectPET = true;
      break;

      case 3:
      selectPETE = true;
      break;
    }

    refreshLCD = true; //Refresh LCD after changing the value of the menu
    refreshSelection = true; //refresh the selection ("X")
  }
}

// change the marker to "X"
void updateSelection()
{
  // When any material is selected, ">" becomes "X"
  if(selectABS==true)
  {
    lcd.setCursor(0,0);
    lcd.print("X");
  }

  if(selectPETG==true)
  {
    lcd.setCursor(0,1);
    lcd.print("X");
  }

  if(selectPET==true)
  {
    lcd.setCursor(0,2);
    lcd.print("X");
  }

  if(selectPETE==true)
  {
    lcd.setCursor(0,3);
    lcd.print("X");
  }
}