#include <PinChangeInterrupt.h>

#define TRIG_PIN 6   
#define ECHO_PIN 2   
#define BUTTON_PIN 3 
#define IR_PIN 4     
#define LED_PIN 13   

volatile int button_count = 0;
volatile int ir_count = 0;
volatile int ultrasonic_count = 0; 
volatile bool led_state = false;
volatile unsigned long echo_start = 0;
volatile unsigned long echo_end = 0;
volatile long duration;
volatile int distance;

void buttonISR() {
    button_count++;
}

void irISR() {
    ir_count++;
}

void ultrasonicISR() {
    
    if (digitalRead(ECHO_PIN) == HIGH) {
    
        echo_start = micros();
    } else {
    
        echo_end = micros();
        duration = echo_end - echo_start;
        distance = duration * 0.034 / 2; 
        
        
        if (distance < 10 && distance > 0) {
            ultrasonic_count++;
            ultrasonicAlertISR();
        }
    }
}

void ultrasonicAlertISR() {
    digitalWrite(LED_PIN, HIGH);
    delay(1000);                
    digitalWrite(LED_PIN, LOW); 
}

void timerISR() {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
}

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(IR_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), ultrasonicISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE);
    attachPCINT(digitalPinToPCINT(IR_PIN), irISR, CHANGE);
    
    cli(); 
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 15624; 
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);   
    TIMSK1 |= (1 << OCIE1A);             
    sei(); 
}

ISR(TIMER1_COMPA_vect) {
    timerISR();
}

void loop() {
    
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    
    Serial.print("Button Count: ");
    Serial.print(button_count);
    Serial.print(" | IR Count: ");
    Serial.print(ir_count);
    Serial.print(" | Ultrasonic Count: ");
    Serial.print(ultrasonic_count);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    delay(500);
}
