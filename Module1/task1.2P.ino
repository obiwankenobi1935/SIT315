#define TRIG_PIN 9
#define ECHO_PIN 2
#define LED_BUILTIN 13

volatile long startTime = 0;
volatile long duration = 0;
bool measuring = false;

void echoISR() {
    if (digitalRead(ECHO_PIN) == HIGH) {
        // Rising edge: Start timing
        startTime = micros();
        measuring = true;
    } else if (measuring) {
        // Falling edge: Calculate duration
        duration = micros() - startTime;
        measuring = false;
    }
}

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);

    // Attach interrupt to the ECHO_PIN (interrupt 0 is for pin 2, interrupt 1 is for pin 3, etc.)
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoISR, CHANGE);
}

void loop() {
    // Trigger ultrasonic pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Wait for measurement to complete
    delay(50);

    // Calculate distance in cm
    int distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Blink LED if object is closer than 10 cm
    if (distance > 0 && distance < 10) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
    }

    delay(500);
}
