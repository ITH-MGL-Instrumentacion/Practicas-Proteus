#include <Arduino.h>

// --- CONFIGURACIÓN COMBO 1 (Pines 2 y 3) ---
const byte pinBoton1 = 2;
const byte pinLed1 = 3;
bool estadoActual1;
bool estadoAnterior1 = HIGH;
bool estadoLed1 = LOW;

// --- CONFIGURACIÓN COMBO 2 (Pines 4 y 5) ---
const byte pinBoton2 = 4;
const byte pinLed2 = 5;
bool estadoBoton2; // Para la lógica simple de encendido directo

// Constantes de lógica inversa (INPUT_PULLUP)
const bool PRESIONADO = LOW;
const bool NO_PRESIONADO = HIGH;

void setup() {
    // Configuración Combo 1
    pinMode(pinBoton1, INPUT_PULLUP);
    pinMode(pinLed1, OUTPUT);
    digitalWrite(pinLed1, estadoLed1);

    // Configuración Combo 2
    pinMode(pinBoton2, INPUT_PULLUP);
    pinMode(pinLed2, OUTPUT);
}

void loop() {
    // ==========================================
    // LÓGICA DEL COMBO 1: Botón Interruptor (Toggle)
    // ==========================================
    estadoActual1 = digitalRead(pinBoton1);

    if (estadoAnterior1 == NO_PRESIONADO && estadoActual1 == PRESIONADO) {
        estadoLed1 = !estadoLed1;
        digitalWrite(pinLed1, estadoLed1);
        delay(50); // Anti-rebote
    }
    estadoAnterior1 = estadoActual1;


    // ==========================================
    // LÓGICA DEL COMBO 2: Encendido directo (Momentáneo)
    // ==========================================
    estadoBoton2 = digitalRead(pinBoton2);

    if (estadoBoton2 == PRESIONADO) {
        digitalWrite(pinLed2, HIGH); // Enciende si se mantiene presionado
    } else {
        digitalWrite(pinLed2, LOW);  // Se apaga al soltarlo
    }
}
