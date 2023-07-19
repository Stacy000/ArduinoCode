// Libraries
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Define pins
#define encoderCLK 9;
#define encoderDT 8;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);  

// Define variables
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void Display(int index)
{             // initialize the lcd 
  lcd.begin(20,4);  // Print a message to the LCD.
  lcd.backlight();
  int Presets_num = 4;
  String menu[Presets_num] = {"ABS", "PETG", "PTE", "PETE"};
  lcd.setCursor(0, 0);
  lcd.print("MENU: SELECT PRESETS");
  lcd.setCursor(0, 2);
  

  for (int i = 0; i < Presets_num; i++) {
    if (i == index) {
      lcd.print(">");
    }
    lcd.print(menu[i]+" ");
  }
}

int ReadEncoder()
{
  currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			counter ++;
			currentDir ="CW";
		}

		Serial.print("Direction: ");
		Serial.print(currentDir);
		Serial.print(" | Counter: ");
		Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(SW);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			Serial.println("Button pressed!");
		}

		// Remember last button press event
		lastButtonPress = millis();
	}

	// Put in a slight delay to help debounce the reading
	delay(1);
}
void setup()
{ 
  lcd.init();            // initialize the lcd 
  int index=3;
  Display(index);
  pinMode(encoderCLK, INPUT);
  pinMode(encoderDT, INPUT);

}


void loop()
{
}
