#include <LiquidCrystal_I2C.h>

#define US_SENSOR A0

LiquidCrystal_I2C lcd(0x27, 16, 2);

// distance between the left and the right wall of the water bottle placement 
int placement_gap = 0;

void setup() {
    pinMode(US_SENSOR, INPUT);

    lcd.init();
    lcd.backlight();    

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TESTING: ");
    lcd.setCursor(0, 1);
    lcd.print("Ultrasonic Sensor");
    
    delay(2000);
}

void loop() {

}

void readPlacementGap() {
    
}