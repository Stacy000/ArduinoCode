#include "DisplayTime.h"

int timeInSec = 0;
int timeInMin = 0;
// Define timer
Timer heatingTimer;
Timer spoolingTimer;

extern LiquidCrystal_I2C lcd;
extern int state;

void DisplayTime()
{
  lcd.setCursor(0,3);

  if(state == 2)
  {
    timeInSec = heatingTimer.read()/1000;
  }

  if(state == 3)
  {
    timeInSec = spoolingTimer.read()/1000;
  }
  
  lcd.print("time: ");

  if(timeInSec > 59)
  {
    timeInMin = timeInSec / 60;
    timeInSec = timeInSec - timeInMin * 60;
    lcd.setCursor(12,3);
    lcd.print(" ");
  }
  lcd.setCursor(5,3);
  lcd.print(timeInMin);
  lcd.setCursor(7,3);
  lcd.print("min ");
  lcd.setCursor(11,3);
  lcd.print(timeInSec);
  lcd.setCursor(13,3);
  lcd.print("s");
}