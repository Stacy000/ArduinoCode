// This Encoder.cpp contains all the functions that is related to the encoder as well as the display page for the material selection stage

#include <Arduino.h>
#include "Encoder.h"

extern int state;
extern bool startMotor;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
extern LiquidCrystal_I2C lcd;

// Define variables for material selection state (state 0)
int menuCounter = 0;
bool selectABS = false;
bool selectPETG = false;
bool selectPLA= false;
bool selectPETE = false;
bool selectNext = false;

// Define variables for state 1
bool selectYes = false;
bool selectBack = false;
bool right = false;
bool left = false;

// Define variables
bool refreshLCD = true;
bool refreshSelection = false;
bool clearSelection = false;

volatile unsigned long lastPushButton = 0;
const unsigned long pushButtonInterval = 500;

// Display the material selection page
void DisplayMaterialSelection()
{
  lcd.setCursor(1,0); //setCursor(column, row)
  lcd.print("ABS");

  lcd.setCursor(1,1);
  lcd.print("PETG");

  lcd.setCursor(1,2);
  lcd.print("PLA");

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
  if(selectABS == true)
  {
    return 0;
  }

  if(selectPETG==true)
  {
    return 1;
  }

  if(selectPLA==true)
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
void UpdateCursorPosition()
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
void Rotate()
{
  int i = digitalRead(encoderDT);
  //Serial.println(i);

  Serial.println("FALLING");
  //Serial.print("\n");
  if(state == 0)
  {
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
    //Serial.print(menuCounter);
  }

  if(state == 1)
  {
    switch(i)
    {
      case 0: 
      left = true;
      right = false;
      break;

      case 1:
      right = true;
      left = false;
      break;
    }
    refreshLCD = true;
  }
}

// Update the selection marker everytime the push button is pressed
void PushButton()
{
  //Serial.println("Button pressed!"); 
  unsigned long currentMillis = millis();

  // Sofware debouncing for push button on the encoder
   if ((currentMillis - lastPushButton) >= pushButtonInterval) 
   { 
    switch(state)
    {
      // When the state is 0
      case 0:
      if(menuCounter == 4)
      {
        selectNext = true;
        state = 1;
      }

      // If any of the material has been selected
      else if(selectABS == true || selectPETG == true || selectPETE == true ||selectPLA == true)
      {
        // Clear the selection
        selectABS = selectPETE = selectPLA = selectPETG = false;
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
        selectPLA = true;
        break;

        case 3:
        selectPETE = true;
        break;
      }

      //Serial.println(state);
      //Serial.println(selectABS);

      refreshLCD = true; //Refresh LCD after changing the value of the menu
      refreshSelection = true; //refresh the selection ("X")

      break;

      // When the state is 1
      case 1:
      if(left == true)
      {
        selectYes = true;
      }

      if(right == true)
      {
        selectBack = true;
      }
      break;
    }

    // Update the time which the push button is pressed
    lastPushButton = currentMillis;
   }
}

// change the marker to "X"
void UpdateSelection()
{
  // When any material is selected, ">" becomes "X"
  if(selectABS == true)
  {
    lcd.setCursor(0,0);
    lcd.print("X");
  }

  if(selectPETG == true)
  {
    lcd.setCursor(0,1);
    lcd.print("X");
  }

  if(selectPLA == true)
  {
    lcd.setCursor(0,2);
    lcd.print("X");
  }

  if(selectPETE == true)
  {
    lcd.setCursor(0,3);
    lcd.print("X");
  }
}

// Update the cursor position to for state 1, left means the cursor is at Yes, right means the cursor is at Back
void UpdateStateOneCursor()
{
  //Serial.print(left);
  if(left == true)
  {
    lcd.setCursor(8,3);
    lcd.print(" ");
    lcd.setCursor(3,3);
    lcd.print(">");
  }

  if(right == true)
  {
    lcd.setCursor(8,3);
    lcd.print(">");
    lcd.setCursor(3,3);
    lcd.print(" ");
  }
}

// When the system is back to state 0, clear all the variables that controls the logic of the program
void ResetAllSelection()
{
  menuCounter = 0;
  selectABS = selectPETE = selectPLA = selectPETG = selectNext = false;
  left = right = selectYes = selectBack = false;
  UpdateCursorPosition();
}