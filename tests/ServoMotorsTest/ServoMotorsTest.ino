#include <Servo.h>

#define GATE_PIN 7
#define FLIPPER_PIN 6

Servo gateServo;
Servo flipperServo;

void setup() {
    gateServo.attach(GATE_PIN);
    flipperServo.attach(FLIPPER_PIN);
}

void loop() {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees in steps of 1 degree
        gateServo.write(pos);              // tell servo to go to position in variable 'pos'
        flipperServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }

    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        gateServo.write(pos);              // tell servo to go to position in variable 'pos'
        flipperServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}