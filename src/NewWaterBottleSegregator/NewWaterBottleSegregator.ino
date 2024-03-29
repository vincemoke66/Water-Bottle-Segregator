#include <LiquidCrystal_I2C.h>
#include <Servo.h>

/* PIN CONFIGURATION */
#define CONVEYOR_RELAY_PIN 8
#define GATE_PIN 9
#define FLIPPER_PIN 10

#define FRONT_TRIG_PIN 2
#define FRONT_ECHO_PIN 3

#define PLASTIC_TRIG_PIN 4
#define PLASTIC_ECHO_PIN 5
#define METAL_TRIG_PIN 6
#define METAL_ECHO_PIN 7

#define CAPACITIVE1_PIN 11
#define CAPACITIVE2_PIN 12

#define INDUCTIVE1_PIN A1
#define INDUCTIVE2_PIN A0

/* LCD PINS */
/* A4 -> SDA */
/* A5 -> SCL */

/* FOR TESTING */
// change the value to "PLASTIC" or "METAL" depending on what 
// bottle material you want to test for segregation
String bottleToSegregate = "METAL"; // SHOULD BE ALL CAPS

// increase or decrease the value depending on the conveyor operation
const int METAL_DURATION = 10000; // in milliseconds
const int PLASTIC_DURATION = 20000; // in milliseconds

const int GATE_OPEN = 180;
const int GATE_CLOSE = 0;
const int FLIPPER_OPEN = 180;
const int FLIPPER_CLOSE = 0;

const int PLACEMENT_THRESHOLD = 20;
const int PLASTIC_BIN_THRESHOLD = 10;
const int METAL_BIN_THRESHOLD = 10;

// DISTANCE VARIABLES
int placement_distance = 0;
int plastic_bin_distance = 0;
int metal_bin_distance = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte binFull[] = { 0x00, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0E };
byte binEmpty[] = { 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E };

byte plasticBottle[] = { 0x00, 0x0C, 0x12, 0x1E, 0x12, 0x1E, 0x1E, 0x1E };
byte metalBottle[] = { 0x00, 0x00, 0x06, 0x09, 0x0F, 0x09, 0x09, 0x06 };

Servo gateServo;
Servo flipperServo;

void setup() {
    Serial.begin(9600);

    // initializing LCD
    lcd.init();
    lcd.backlight();
    
    // creating trash bin characters
    lcd.createChar(0, binEmpty);
    lcd.createChar(1, binFull);
    lcd.createChar(2, plasticBottle);
    lcd.createChar(3, metalBottle);

    // sets conveyor relay as output
    pinMode(CONVEYOR_RELAY_PIN, OUTPUT);

    // sets all trigger pin of ultrasonic sensors as output
    pinMode(FRONT_TRIG_PIN, OUTPUT);
    pinMode(PLASTIC_TRIG_PIN, OUTPUT);
    pinMode(METAL_TRIG_PIN, OUTPUT);

    // sets all echo pin of ultrasonic sensors as input 
    pinMode(FRONT_ECHO_PIN, INPUT);
    pinMode(PLASTIC_ECHO_PIN, INPUT);
    pinMode(METAL_ECHO_PIN, INPUT);

    // sets two capacitive sensors as inputs
    pinMode(CAPACITIVE1_PIN, INPUT);
    pinMode(CAPACITIVE2_PIN, INPUT);

    // attaches gate and flipper servo pins to specific servo objects
    gateServo.attach(GATE_PIN);
    flipperServo.attach(FLIPPER_PIN);

    digitalWrite(CONVEYOR_RELAY_PIN, HIGH);

    // initially closes gate and flipper 
    gateServo.write(GATE_CLOSE);
    flipperServo.write(FLIPPER_CLOSE);
    delay(500);
}

void loop() {
    // read trash bins capacity 
    readMetalTrashBinDistance();
    readPlasticTrashBinDistance();
    Serial.print("Plastic bin distance: "); // for debugging 
    Serial.println(plastic_bin_distance); 
    Serial.print("Metal bin distance: "); // for debugging 
    Serial.println(metal_bin_distance); 

    // display lcd for inserting bottle 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("PLEASE INSERT");
    lcd.setCursor(3, 1);
    lcd.print("A BOTTLE ");
    lcd.setCursor(12, 1);
    lcd.write(2); // writes plastic bottle symbol
    lcd.setCursor(13, 1);
    lcd.write(3); // writes plastic bottle symbol

    // check for bottle placement
    readPlacementDistance();
    Serial.print("Placement distance: "); // for debugging 
    Serial.println(placement_distance); 

    // if bottle has not been placed
    if (placement_distance >= PLACEMENT_THRESHOLD) return;
    delay(4000);

    Serial.println("Bottle has been placed!"); // for debugging 
    Serial.println("Identifying bottle..."); // for debugging 

    // display lcd for identifying bottle 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("IDENTIFYING");
    lcd.setCursor(5, 1);
    lcd.print("BOTTLE");

    // if bottle has been placed
    delay(1500); // added a delay for proper identification
    identifyBottle();


    Serial.println("Bottle identified!"); // for debugging 
    Serial.print("Material: "); // for debugging 
    Serial.println(bottleToSegregate); // for debugging 

    // if bottle to segregate is plastic and plastic bin is full
    if (plastic_bin_distance < PLASTIC_BIN_THRESHOLD && bottleToSegregate == "PLASTIC") {
        while (placement_distance < PLACEMENT_THRESHOLD) {
            lcd.clear();
            displayTrashBinFullWarning(1);
        }
        return;
    }
    // if bottle to segregate is metal and metal bin is full
    if (metal_bin_distance < METAL_BIN_THRESHOLD && bottleToSegregate == "METAL") {
        while (placement_distance < PLACEMENT_THRESHOLD) {
            lcd.clear();
            displayTrashBinFullWarning(0);
        }
        return;
    }

    Serial.println("\n\nSegregating bottle..."); // for debugging 

    // display lcd for segregating bottle with correct identified material
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SEGREGATING");
    lcd.setCursor(4, 1);
    lcd.print(bottleToSegregate);

    Serial.print("Is metal: ");
    Serial.println(bottleToSegregate == "METAL");

    segregate(bottleToSegregate == "METAL");
    delay(2000);

    Serial.println("\n\nBottle segregated!"); // for debugging 

    // closing gate and flipper in case did not closed
    gateServo.write(GATE_CLOSE);
    flipperServo.write(FLIPPER_CLOSE);

    // display lcd for segregating bottle complete
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SEGREGATION");
    lcd.setCursor(3, 1);
    lcd.print("COMPLETE!");
    delay(2000);
}

void displayTrashBinFullWarning(int isPlastic) {
    if (isPlastic) {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("PLASTIC BIN");
    }
    else {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("METAL BIN");
    }

    lcd.setCursor(4, 1);
    lcd.print("FULL ");

    // flashing empty and full bin animation
	for (int i = 0; i < 3; i++) {
		lcd.setCursor(10, 1);
		lcd.write(0);
		delay(500);

		lcd.setCursor(10, 1);
		lcd.write(1);
		delay(500);
	}

	delay(500);
}

void identifyBottle() {
    // if capacitive is 1, then it is metal
    int bottleCapacitive = digitalRead(CAPACITIVE1_PIN) + digitalRead(CAPACITIVE2_PIN);

    Serial.print("Capacitive value: ");
    Serial.print(bottleCapacitive);

    if (bottleCapacitive != 0) {
        bottleToSegregate = "METAL";
        return;
    }

    bottleToSegregate = "PLASTIC";
}

void readPlacementDistance() {
    digitalWrite(FRONT_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(FRONT_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(FRONT_TRIG_PIN, LOW);

    int duration = pulseIn(FRONT_ECHO_PIN, HIGH);

    placement_distance = duration * 0.034 / 2;
}

void readMetalTrashBinDistance() {
    digitalWrite(METAL_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(METAL_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(METAL_TRIG_PIN, LOW);

    int metal_duration = pulseIn(METAL_ECHO_PIN, HIGH);

    metal_bin_distance = metal_duration * 0.034 / 2;
    delay(10);
}

void readPlasticTrashBinDistance() {
    digitalWrite(PLASTIC_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(PLASTIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(PLASTIC_TRIG_PIN, LOW);

    int plastic_duration = pulseIn(PLASTIC_ECHO_PIN, HIGH);

    plastic_bin_distance = plastic_duration * 0.034 / 2;
    delay(10);
}

void segregate(bool isMetal) {
    // open gate
    gateServo.write(GATE_OPEN);
    delay(500);

    // if metal, open flipper
    if (isMetal) flipperServo.write(FLIPPER_OPEN);

    delay(200);

    // sets the needed duration for conveyor to segregate based on the bottle material
    int duration = METAL_DURATION;
    if (!isMetal) duration = PLASTIC_DURATION;

    // operate conveyor
    digitalWrite(CONVEYOR_RELAY_PIN, LOW);

    // partial delay for closing gate 
    delay(3000);

    gateServo.write(GATE_CLOSE);
    delay(500);

    // continuation delay for the conveyor to segregate 
    delay(duration - 3000);

    // turns off conveyor after the segregation duration
    digitalWrite(CONVEYOR_RELAY_PIN, HIGH);

    // always closes the gate after segregating 
    gateServo.write(GATE_CLOSE);

    // if bottle material is metal, closes back the flipper
    if (isMetal) flipperServo.write(FLIPPER_CLOSE);

    delay(200);
}