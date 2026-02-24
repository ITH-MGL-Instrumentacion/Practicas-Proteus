#include <Arduino.h>

byte ledPin = 3;
byte analogPin = A0;
int adcValue;
byte pwmValue;

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    adcValue = analogRead(analogPin);
    pwmValue = map(adcValue, 0, 1023, 0, 255);
    analogWrite(ledPin, pwmValue);
    delay(10);
}