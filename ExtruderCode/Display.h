// Libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Define pins for rotary encoder for the menu
#define encoderCLK 3 //Interrupt pin
#define encoderDT 31
#define encoderSW 2 //Interrupt pin

void ClearLCD();
void DisplayMaterialSelection();
void ClearCurrentCursor(int num);
int CheckCurrentSelection();
void updateCursorPosition();
void rotate();
void pushButton();
void updateSelection();


