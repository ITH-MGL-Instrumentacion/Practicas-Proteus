#include <Arduino.h>

byte pinBoton = 2;
byte pinLed = 3;

// La variable estadoBoton se declara como bool 
// porque solo necesitamos saber si el botón 
// está presionado o no.
bool estadoBoton;

// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO = HIGH; // El botón no está presionado cuando el pin está en HIGH


void setup() {
	pinMode(pinBoton, INPUT_PULLUP);
	pinMode(pinLed, OUTPUT);
}

void loop() {
	estadoBoton = digitalRead(pinBoton);

    // La lógica del botón es inversa porque se usa INPUT_PULLUP,
    // el pin se mantiene en HIGH (5v) cuando el botón no está presionado
	if (estadoBoton == PRESIONADO) {
		digitalWrite(pinLed, HIGH);
	} else {
		digitalWrite(pinLed, LOW);
	}
}
