#include <Arduino.h>

// ---------------------------------------------------------
// PRÁCTICA BASE: ENTRADA ANALÓGICA -> SALIDA DIGITAL DIRECTA (10 bits)
//
// Objetivo:
// - Leer el potenciómetro en A0 (ADC de 10 bits: 0 a 1023).
// - Mostrar el valor leído en binario usando 10 LEDs.
// - Cada LED representa 1 bit del número leído.
//
// Conexión sugerida:
// - Potenciómetro: extremos a 5V y GND, cursor a A0.
// - LEDs: pines digitales 4 a 13 (cada LED con resistencia).
// ---------------------------------------------------------

// Pin analógico donde se conecta el cursor del potenciómetro.
const byte pinPot = A0;

// Arreglo con los 10 pines de salida para los 10 LEDs.
// ledPins[0] mostrará el bit menos significativo (bit 0)
// y ledPins[9] mostrará el bit más significativo (bit 9).
const byte ledPins[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// Variable donde guardamos la lectura del ADC (0 a 1023).
int lecturaCAD = 0;

void setup() {
    // Configuramos cada pin del arreglo como salida digital.
    for (byte i = 0; i < 10; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    // 1) Leer la entrada analógica de A0.
    //    analogRead() devuelve un número entero de 10 bits (0..1023).
    lecturaCAD = analogRead(pinPot);
    int cantidadEncendida = map(lecturaCAD, 0, 1023, 0, 10);
    // 2) Separar el valor leído en bits y mostrar cada bit en un LED.
    //    Recorremos los 10 bits del número (bit 0 a bit 9).
    for (int i = 0; i < 10; i++) {
    if (i < cantidadEncendida) {
        digitalWrite(ledPins[i], HIGH);  // Encender
    } else {
        digitalWrite(ledPins[i], LOW);   // Apagar
    }
}

    // Pequeño retardo para estabilidad visual.
    delay(5);
}
