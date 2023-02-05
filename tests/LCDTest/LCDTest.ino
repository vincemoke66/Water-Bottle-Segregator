/* www.electronicsprojectshub.com */
/*Import following Libraries*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long int current_time;
unsigned long int waiting_time = millis();
unsigned long int notWaiting_time = millis();

bool isWaiting = 1;
void setup() 
{
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
}

void loop() 
{
  current_time = millis();

  if (isWaiting) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Waiting..."); 
    lcd.setCursor(0,1);
    lcd.print("Like | Share");

    if (current_time - notWaiting_time > 5000) {
      isWaiting = false;
    }
    waiting_time = current_time;
  } 

  if(!isWaiting) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Not Waiting "); 
    lcd.setCursor(0,1);
    lcd.print("Like | Share");
    
    if (current_time - waiting_time > 5000) {
      current_time = millis();
      isWaiting = true;
    }
    notWaiting_time = current_time;
  }
}