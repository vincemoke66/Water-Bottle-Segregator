/* INCLUSIONG OF NECESSARY LIBRARIES*/
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

/* PIN CONFIGURATIONS */ 
#define FRONT_TRIG_PIN   2 // **
#define FRONT_ECHO_PIN   3 // ULTRASONIC SENSORS
#define PLASTIC_TRIG_PIN 4 //   
#define PLASTIC_ECHO_PIN 5 // 
#define METAL_TRIG_PIN   6 // 
#define METAL_ECHO_PIN   7 // **

#define INDUCTIVE1_PIN    8     // **
#define INDUCTIVE2_PIN    9     // INDUCTIVE and
#define CAPACITIVE1_PIN   10    // CONDUCTIVE SENSORS
#define CAPACITIVE2_PIN   11    // **

#define CONVEYOR_RELAY_PIN  12

#define GATE_SERVO_PIN      13
#define FLIPPER_SERVO_PIN   6

/* CONSTANTS */
const int PLASTIC_DURATION = 3000;
const int METAL_DURATION = 5000;

const int PLACEMENT_BOUNDARY = 25;

const int PLASTIC_FULL = 28;
const int METAL_FULL = 28;

int placement_gap = 0;
int plastic_bin_gap = 0;
int metal_bin_gap = 0;

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

/* SYSMTE TIME VARIABLES */
unsigned long waiting_time = 0; 
unsigned long identifying_time = 0; 
unsigned long segregating_time = 0;

/* BOOLEANS */
bool hasDisplayed = false;

/* DEFINING SERVO MOTOR OBJECTS */
Servo gateServo;
Servo flipperServo;

void setup() {
    // Setting input sensors 
    pinMode(INDUCTIVE1_PIN, INPUT);
    pinMode(INDUCTIVE2_PIN, INPUT);
    pinMode(CAPACITIVE1_PIN, INPUT);
    pinMode(CAPACITIVE2_PIN, INPUT);
    pinMode(FRONT_ECHO_PIN, INPUT);
    pinMode(PLASTIC_ECHO_PIN, INPUT);
    pinMode(METAL_ECHO_PIN, INPUT);

    // Setting output/actuators as output
    pinMode(FRONT_TRIG_PIN, OUTPUT);
    pinMode(PLASTIC_TRIG_PIN, OUTPUT);
    pinMode(METAL_TRIG_PIN, OUTPUT);
    pinMode(CONVEYOR_RELAY_PIN, OUTPUT);

    // Attaching servo pins to servo objects 
    gateServo.attach(GATE_SERVO_PIN);
    flipperServo.attach(GATE_SERVO_PIN);

    // initializing status and material 
    status = WAITING;
    material = METAL;

    // lcd initialization
    lcd.init();
    lcd.backlight();
}

void loop() {
    /// WAITING STATUS
    if (status == WAITING) {
        if (!hasDisplayed) {
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("PLEASE INSERT");
            lcd.setCursor(4, 1);
            lcd.print("A BOTTLE.");
            hasDisplayed = true;
        }

        readPlacementGap();
        if (placement_gap < PLACEMENT_BOUNDARY) {
            status = IDENTIFYING;
            hasDisplayed = false;            
        }
        
        waiting_time = millis();
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

        // sets 2-second delay for lesser error of bottle identification
        if (millis() - waiting_time > 2000) {
            material = (digitalRead(INDUCTIVE1_PIN) > 0) ? METAL : PLASTIC;
            status = SEGREGATIING;
            hasDisplayed = false;
        }

        identifying_time = millis();
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

        unsigned long interval = millis() - identifying_time;
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
        segregating_time = millis();
    }
}

void readSensors() {
    // read front ultrasonic sensor
    // read plastic ultrasonic sensor
    // read metal ultrasonic sensor
    // read inductive1 sensor
    // read inductive2 sensor
    // read conductive1 sensor
    // read conductive2 sensor
}

void readPlacementGap() {
    digitalWrite(FRONT_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(FRONT_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(FRONT_TRIG_PIN, LOW);

    int front_duration = pulseIn(FRONT_ECHO_PIN, HIGH);

    placement_gap = front_duration * 0.034 / 2;
}

void readTrashBinsDistance() {
    digitalWrite(PLASTIC_TRIG_PIN, LOW);
    digitalWrite(METAL_TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(PLASTIC_TRIG_PIN, HIGH);
    digitalWrite(METAL_TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(PLASTIC_TRIG_PIN, LOW);
    digitalWrite(METAL_TRIG_PIN, LOW);

    int plastic_duration = pulseIn(FRONT_ECHO_PIN, HIGH);
    int metal_duration = pulseIn(FRONT_ECHO_PIN, HIGH);

    plastic_bin_gap = plastic_duration * 0.034 / 2;
    metal_bin_gap = metal_bin_gap * 0.034 / 2;
}