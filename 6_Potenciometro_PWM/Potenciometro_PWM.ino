#include <Arduino.h>

const byte pwmPin = 3;          // LED controlado por PWM en D3
const byte potPin = A0;         // Entrada analógica del potenciómetro
const byte ledPins[] = {4,5,6,7,8,9,10,11,12,13}; // LEDs en los pines digitales 4..13
const byte numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void setup() {
    pinMode(pwmPin, OUTPUT);
    for (byte i = 0; i < numLeds; ++i) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

void loop() {
    // leer el potenciómetro (0..1023)
    int lectura = analogRead(potPin);

    // convertir a ciclo de trabajo PWM (0..255)
    byte valorPWM = map(lectura, 0, 1023, 0, 255);
    analogWrite(pwmPin, valorPWM);

    // calcular cuántos LEDs encender
    byte ledsEncendidos = map(lectura, 0, 1023, 0, numLeds);
    for (byte i = 0; i < numLeds; ++i) {
        if (i < ledsEncendidos) {
            digitalWrite(ledPins[i], HIGH);
        } else {
            digitalWrite(ledPins[i], LOW);
        }
    }

    // pequeña pausa para evitar parpadeo brusco
    delay(50);
}