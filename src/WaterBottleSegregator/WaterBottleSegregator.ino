#include <LiquidCrystal_I2C.h>

// INPUT DEFINITIONS
#define ULTRASONIC    A0 // TODO: update ultrasonic sensor pins to 2 pins
#define INDUCTIVE1    0
#define INDUCTIVE2    1
#define CAPACITIVE1   2
#define CAPACITIVE2   3

// OUTPUT DEFINITIONS
#define CONVEYOR  4
#define MOTOR1    5
#define MOTOR2    6

// RANGE AND DURATION DEFINITIONS
#define CD_PLASTIC  3000
#define CD_METAL    5000
#define BOUNDARY    1000

enum machine_stats {
  ERROR,
  WAITING,
  IDENTIFYING,
  SEGREGATIING
} status;

enum materials {
  UNKNOWN,
  PLASTIC,
  METAL
} material;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// TIME 
unsigned long current_time; 
unsigned long waiting_time = millis(); 
unsigned long identifying_time = millis(); 
unsigned long segregating_time = millis();

bool hasDisplayed = false;

void setup() {
  pinMode(ULTRASONIC, INPUT);
  pinMode(INDUCTIVE1, INPUT);
  pinMode(INDUCTIVE2, INPUT);
  pinMode(CAPACITIVE1, INPUT);
  pinMode(CAPACITIVE2, INPUT);

  pinMode(CONVEYOR, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  status = WAITING;
  material = METAL;

  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
}

void loop() {
  current_time = millis();

  /// WAITING STATUS
  if (status == WAITING) {
    if (!hasDisplayed) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Waiting for");
      lcd.setCursor(3, 1);
      lcd.print("Bottle...");
      hasDisplayed = true;
    }

    // if (analogRead(ULTRASONIC) < BOUNDARY) {
    // TODO: update proper ultrasonic sensor readings 
    
    if (current_time - segregating_time > 5000) {
      status = IDENTIFYING;
      hasDisplayed = false;
    }
    waiting_time = current_time;
  }

  /// IDENTIFYING
  if (status == IDENTIFYING) {
    if (!hasDisplayed) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Identifying");
      lcd.setCursor(3, 1);
      lcd.print("Bottle...");
      hasDisplayed = true;
    }

    if (current_time - waiting_time > 2000) {
      material = (digitalRead(INDUCTIVE1) > 0) ? METAL : PLASTIC;
      status = SEGREGATIING;
      hasDisplayed = false;
    }
    identifying_time = current_time;
  }

  /// SEGREGATING METAL
  if (status == SEGREGATIING) {
    if (!hasDisplayed) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Segregating");
      lcd.setCursor(2, 1);
      const char* content = (material == METAL) ? "METAL BOTTLE" : "PLASTIC BOTTLE"; 
      lcd.print(content);
      hasDisplayed = true;
    }

    unsigned long interval = current_time - identifying_time;
    if ((interval > CD_METAL && material == METAL) || (interval > CD_PLASTIC && material == PLASTIC)) {
      status = WAITING;
      hasDisplayed = false;
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("BOTTLE");
      lcd.setCursor(3, 1);
      lcd.print("SEGREGATED!");
      delay(2000);
    }
    segregating_time = current_time;
  }
}