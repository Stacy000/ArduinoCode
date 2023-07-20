int timeInSec = 0;
int timeInMin = 0;

void DisplayTime()
{
  lcd.setCursor(0,3);
  timeInSec = timer.read()/1000;
  lcd.print("time: ");

  if(timeInSec > 59)
  {
    timeInMin = timeInSec / 60;
    timeInSec = timeInSec - timeInMin * 60;
  }
  lcd.setCursor(5,3);
  lcd.print(timeInMin);
  lcd.setCursor(7,3);
  lcd.print("min ");
  lcd.setCursor(11,3);
  lcd.print(timeInSec);
  lcd.print("s");
}