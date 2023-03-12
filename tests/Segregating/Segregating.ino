#include <Servo.h>

#define CONVEYOR_RELAY_PIN 7
#define GATE_PIN 6
#define FLIPPER_PIN 5

// change the value to "PLASTIC" or "METAL" depending on what 
// bottle material you want to test for segregation
String bottleToSegregate = "METAL"; // SHOULD BE ALL CAPS

// increase or decrease the value depending on the conveyor operation
const int METAL_DURATION = 10000; // in milliseconds
const int PLASTIC_DURATION = 5000; // in milliseconds

const int GATE_OPEN = 180;
const int GATE_CLOSE = 0;
const int FLIPPER_OPEN = 180;
const int FLIPPER_CLOSE = 0;

Servo gateServo;
Servo flipperServo;

void setup() {
    Serial.begin(9600);

    pinMode(CONVEYOR_RELAY_PIN, OUTPUT);

    gateServo.attach(GATE_PIN);
    flipperServo.attach(FLIPPER_PIN);

    // initially closes gate and flipper
    gateServo.write(GATE_CLOSE);
    flipperServo.write(FLIPPER_CLOSE);
    delay(500);
}

void loop() {
    segregate(bottleToSegregate == "METAL");
    delay(2000);
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
    delay(duration);

    // turns off conveyor after the segregation duration
    digitalWrite(CONVEYOR_RELAY_PIN, HIGH);

    // always closes the gate after segregating 
    gateServo.write(GATE_CLOSE);

    // if bottle material is metal, closes back the flipper
    if (isMetal) flipperServo.write(FLIPPER_CLOSE);

    delay(200);
}