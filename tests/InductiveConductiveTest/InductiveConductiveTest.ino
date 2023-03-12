#define INDUCTIVE1_PIN 7
#define INDUCTIVE2_PIN 6
#define CONDUCTIVE1_PIN 5
#define CONDUCTIVE2_PIN 4

void setup() {
    Serial.begin(9600);

    pinMode(INDUCTIVE1_PIN, INPUT);
    pinMode(INDUCTIVE2_PIN, INPUT);
    pinMode(CONDUCTIVE1_PIN, INPUT);
    pinMode(CONDUCTIVE2_PIN, INPUT);
}

void loop() {
    Serial.println("- - - - - - - ");
    Serial.print("Inductive 1: ");
    Serial.println(digitalRead(INDUCTIVE1_PIN));
    Serial.print("Inductive 2: ");
    Serial.println(digitalRead(INDUCTIVE2_PIN));

    Serial.print("Conductive 1: ");
    Serial.print(digitalRead(CONDUCTIVE1_PIN));
    Serial.print("Conductive 2: ");
    Serial.println(digitalRead(CONDUCTIVE2_PIN));
    Serial.println("- - - - - - - ");
}