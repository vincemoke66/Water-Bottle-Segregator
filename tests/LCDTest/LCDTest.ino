#include <LiquidCrystal_I2C.h>

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* CUSTOM CHARACTERS */
byte binFull[] = { 0x00, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0E };
byte binEmpty[] = { 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E };
byte plasticBottle[] = { 0x00, 0x0C, 0x12, 0x1E, 0x12, 0x1E, 0x1E, 0x1E };
byte metalBottle[] = { 0x00, 0x00, 0x06, 0x09, 0x0F, 0x09, 0x09, 0x06 };

void setup() 
{
  lcd.init();
  lcd.backlight();

  // adding custom characters 
  lcd.createChar(0, binEmpty);
  lcd.createChar(1, binFull);
  lcd.createChar(2, plasticBottle);
  lcd.createChar(3, metalBottle);
}

void loop() 
{
  lcd.setCursor(2, 0);
  lcd.print("PLEASE INSERT");
  lcd.setCursor(3, 1);
  lcd.print("A BOTTLE ");
  lcd.write(2);
  lcd.write(3);
}