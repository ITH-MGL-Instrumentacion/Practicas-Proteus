#include <Arduino.h>
#include <math.h>

const byte pinPwm = 5;
const byte pinPotFrecuencia = A0;
const byte pinPotAmplitud = A1;

// Tiempo de muestreo fijo para actualizar la PWM modulada
const unsigned long Ts_us = 100;   // 100 us
const float dt = Ts_us * 1e-6;     // en segundos

// Leer controles cada 1 segundo
const unsigned long Tcontrol_ms = 1000;

unsigned long ultimoMuestreo = 0;
unsigned long ultimoControl = 0;

float frecuencia = 10.0;   // Hz
float amplitud = 64.0;     // 0 a 127
float fase = 0.0;

void setup() {
    pinMode(pinPwm, OUTPUT);
}

void loop() {
    unsigned long ahoraUs = micros();
    unsigned long ahoraMs = millis();

    // 1) Actualizar frecuencia y amplitud cada segundo
    if (ahoraMs - ultimoControl >= Tcontrol_ms) {
        ultimoControl += Tcontrol_ms;

        int lecturaFreq = analogRead(pinPotFrecuencia);
        int lecturaAmp = analogRead(pinPotAmplitud);

        // Rango razonable para PWM de 980 Hz
        frecuencia = 1.0 + (39.0 * lecturaFreq / 1023.0);   // 1 a 40 Hz

        // Amplitud máxima 127 para no recortar
        amplitud = 127.0 * lecturaAmp / 1023.0;
    }

    // 2) Generar la señal en instantes fijos
    if (ahoraUs - ultimoMuestreo >= Ts_us) {
        ultimoMuestreo += Ts_us;

        fase += 2.0 * PI * frecuencia * dt;

        if (fase >= 2.0 * PI) {
            fase -= 2.0 * PI;
        }

        float valor = 127.5 + amplitud * sin(fase);

        if (valor < 0.0) {
            valor = 0.0;
        }
        if (valor > 255.0) {
            valor = 255.0;
        }

        analogWrite(pinPwm, (byte)valor);
    }
}