#include <Arduino.h>

byte pinBoton1 = 2;
byte pinLed1 = 3;
byte pinBoton2 = 4;
byte pinLed2 = 5;

bool estadoAnterior1 = HIGH;
bool estadoAnterior2 = HIGH;

// La variable estadoBoton se declara como bool 
// porque solo necesitamos saber si el botón 
// está presionado o no.
bool estadoActual1;
bool estadoActual2;
bool estadoLed1 = LOW;
bool estadoLed2 = LOW;

// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO1 = LOW; // El botón está presionado cuando el pin está en LOW
const bool PRESIONADO2 = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO1 = HIGH; // El botón no está presionado cuando el pin está en HIGH
const bool NO_PRESIONADO2 = HIGH; // El botón no está presionado cuando el pin está en HIGH


void setup() {
	pinMode(pinBoton1, INPUT_PULLUP);
	pinMode(pinLed1, OUTPUT);
	pinMode(pinBoton2, INPUT_PULLUP);
	pinMode(pinLed2, OUTPUT);
	pinMode(pinBoton1, INPUT_PULLUP);
	pinMode(pinLed1, OUTPUT);
	pinMode(pinBoton2, INPUT_PULLUP);
	pinMode(pinLed2, OUTPUT);
}

void loop() {
	 // Recordemos cambiar el nombre del botón
		estadoActual1 = digitalRead(pinBoton1);
		estadoActual2 = digitalRead(pinBoton2);
    // Recordemos que usa lógica negativa, así que dice que si el estado anterior del botón era apagado (HIGH) y el estado 
    if (estadoAnterior1 == NO_PRESIONADO1 && estadoActual1 == PRESIONADO1) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed1 = !estadoLed1;
        digitalWrite(pinLed1, estadoLed1);
        delay(20);
    }
	if (estadoAnterior2 == NO_PRESIONADO2 && estadoActual2 == PRESIONADO2) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed2 = !estadoLed2;
        digitalWrite(pinLed2, estadoLed2);
        delay(20);
    }

    estadoAnterior1 = estadoActual1;
    estadoAnterior2 = estadoActual2;
}