#include <Arduino.h>
#include <math.h>

byte pinLed = 3;
const float periodoMs = 2000.0; // 2 segundos para un ciclo completo

void setup() {
    pinMode(pinLed, OUTPUT);
}

void loop() {
    unsigned long t = millis();

    // Ángulo en radianes para un periodo de 2 segundos
    float angulo = 2.0 * PI * (t % (unsigned long)periodoMs) / periodoMs;

    // Normalizar seno de [-1,1] a [0,255]
    float senoNormalizado = (sin(angulo) + 1.0) * 0.5;
    byte valorPWM = (byte)(senoNormalizado * 255.0);

    analogWrite(pinLed, valorPWM);

    delay(5); // suaviza la transición
}
