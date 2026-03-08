#include <Arduino.h>

byte pinBoton = 2;
byte pinLed = 3;
byte pinBoton2 = 4;
byte pinLed2 = 5;

// Variables para el primer botón y LED
bool estadoActual;
bool estadoLed = LOW;
bool estadoAnterior = HIGH;

// Variables para el segundo botón y LED
bool estadoActual2;
bool estadoLed2 = LOW;
bool estadoAnterior2 = HIGH;

// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO = HIGH; // El botón no está presionado cuando el pin está en HIGH

void setup() {
    pinMode(pinBoton, INPUT_PULLUP);
    pinMode(pinLed, OUTPUT);
    pinMode(pinBoton2, INPUT_PULLUP);
    pinMode(pinLed2, OUTPUT);
}

void loop() {
    // Lógica para el primer botón y LED
    estadoActual = digitalRead(pinBoton);
    if (estadoAnterior == NO_PRESIONADO && estadoActual == PRESIONADO) {
        estadoLed = !estadoLed;
        digitalWrite(pinLed, estadoLed);
        delay(20);
    }
    estadoAnterior = estadoActual;

    // Lógica para el segundo botón y LED
    estadoActual2 = digitalRead(pinBoton2);
    if (estadoAnterior2 == NO_PRESIONADO && estadoActual2 == PRESIONADO) {
        estadoLed2 = !estadoLed2;
        digitalWrite(pinLed2, estadoLed2);
        delay(20);
    }
    estadoAnterior2 = estadoActual2;
}