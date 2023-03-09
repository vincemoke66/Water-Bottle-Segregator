#define TRIG_PIN 9
#define ECHO_PIN 10

int placement_gap = 0;

void setup() {
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    readPlacementGap();

    Serial.println(placement_gap);
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