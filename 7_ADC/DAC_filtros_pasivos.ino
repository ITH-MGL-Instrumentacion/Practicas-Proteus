#include <Arduino.h>
#include <math.h>

byte pinLed = 5;
const float frecuencia = 50.0;
const float dt = 0.0001;   // 0.1 ms
float fase = 0.0;

void setup() {
    pinMode(pinLed, OUTPUT);
}

void loop() {
    fase += 2.0 * PI * frecuencia * dt;

    if (fase >= 2.0 * PI) {
        fase -= 2.0 * PI;
    }

    float senoNormalizado = (sin(fase) + 1.0) * 0.5;
    byte valorPWM = (byte)(senoNormalizado * 255.0);

    analogWrite(pinLed, valorPWM);

    delayMicroseconds(1000*dt);
}