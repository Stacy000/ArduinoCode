
void setup() 
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);

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

  // Store states of the rotary encoder
  // CLKPrev = digitalRead(encoderCLK);
  // DTPrev = digitalRead(encoderDT);
  
  // Define the interrupt functions
  attachInterrupt(digitalPinToInterrupt(encoderCLK), rotate, FALLING);
  attachInterrupt(digitalPinToInterrupt(encoderSW), pushButton, FALLING);

  // Set the initial state to 0
  state = 0;
}

void loop() {

  // Put your main code here, to run repeatedly:
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
    delay(5000);
    state = 2;
  }

  if(state==2)
  {
    ClearLCD();
    lcd.setCursor(0, 0);
    lcd.print("heating...");

    float temp= GetTemperature();
    lcd.setCursor(0, 1);
    lcd.print(temp);
    delay(1000);
  }
}
