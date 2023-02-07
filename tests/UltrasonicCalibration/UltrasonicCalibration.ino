#include <LiquidCrystal_I2C.h>

// PIN DEFINITIONS
#define US_SENSOR A0
#define TRIG_PIN 9
#define ECHO_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

// distance between the left and the right wall of the water bottle placement 
int placement_gap = 0;

void setup() {
    pinMode(US_SENSOR, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

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
    lcd.clear();

    readPlacementGap();
    showPlacementGap();

    delay(1000);
}

void readPlacementGap() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    int duration = pulseIn(ECHO_PIN, HIGH);

    placement_gap = duration * 0.034 / 2;
}

void showPlacementGap() {
    lcd.setCursor(0, 0);
    lcd.print("Gap: ");
    lcd.setCursor(5, 1);
    lcd.print(placement_gap);
}