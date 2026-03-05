#include <Arduino.h>

byte pinBoton1 = 2;
byte pinLed1 = 3;
byte estadoActual1;
bool estadoAnterior1 = HIGH;
bool estadoLed1 = LOW;

byte pinBoton2 = 4;
byte pinLed2 = 5;
byte estadoActual2;
bool estadoAnterior2 = HIGH;
bool estadoLed2 = LOW;


// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO = HIGH; // El botón no está presionado cuando el pin está en HIGH

void setup() {
	pinMode(pinBoton1, INPUT_PULLUP);
	pinMode(pinLed1, OUTPUT);
	pinMode(pinBoton2, INPUT_PULLUP);
	pinMode(pinLed2, OUTPUT);
}

void loop() {
	// Recordemos cambiar el nombre del botón
    estadoActual1 = digitalRead(pinBoton1);
    // El if es para
    if (estadoAnterior1 == NO_PRESIONADO && estadoActual1 == PRESIONADO) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed1 = !estadoLed1;
        digitalWrite(pinLed1, estadoLed1);
        delay(20);
    }

    estadoAnterior1 = estadoActual1;   
    
    // Recordemos cambiar el nombre del botón
    estadoActual2 = digitalRead(pinBoton2);
    // El if es para
    if (estadoAnterior2 == NO_PRESIONADO && estadoActual2 == PRESIONADO) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed2 = !estadoLed2;
        digitalWrite(pinLed2, estadoLed2);
        delay(20);
    }

    estadoAnterior2 = estadoActual2;
    
}


