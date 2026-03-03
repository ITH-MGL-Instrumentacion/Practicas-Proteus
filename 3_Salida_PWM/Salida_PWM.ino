#include <Arduino.h>
#include <math.h>

byte pinLed = 3;
const float periodoMs = 1000.0;

void setup() 
{
    pinMode(pinLed, OUTPUT);
}

void loop() 
{
    unsigned long t = millis();

    // Ángulo en radianes para un periodo de 1 segundo
    float angulo = 2.0 * PI * (t % (unsigned long)periodoMs) / periodoMs;

    // Escalar de [-1, 1] a [0, 255]
    float senoNormalizado = (sin(angulo) + 1.0) * 0.5;
    byte valorPWM = (byte)(senoNormalizado * 255.0);

    analogWrite(pinLed, valorPWM);
    delay(5);
}