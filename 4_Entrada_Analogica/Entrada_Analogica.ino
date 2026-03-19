#include <Arduino.h>

const byte pinPot = A0;
const byte pinPwmLed = 3;
const byte ledPins[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

void setup() {
    pinMode(pinPwmLed, OUTPUT);
    for (byte i = 0; i < 10; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    int lectura = analogRead(pinPot);

    int valorPwm = map(lectura, 0, 1023, 0, 255);
    analogWrite(pinPwmLed, valorPwm);

    byte ledsEncendidos = map(lectura, 0, 1023, 0, 10);

    for (byte i = 0; i < 10; i++) {
        digitalWrite(ledPins[i], (i < ledsEncendidos) ? HIGH : LOW);
    }

    delay(5);
}
