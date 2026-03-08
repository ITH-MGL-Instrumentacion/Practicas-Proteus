#include <Arduino.h>

byte pinBoton = 2;
byte pinLed = 3;

// La variable estadoBoton se declara como bool 
// porque solo necesitamos saber si el botón 
// está presionado o no.
bool estadoActual;
bool estadoLed = LOW;
bool estadoAnterior = HIGH;

// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO = HIGH; // El botón no está presionado cuando el pin está en HIGH


void setup() {
	pinMode(pinBoton, INPUT_PULLUP);
	pinMode(pinLed, OUTPUT);
}

void loop() {
	
	 // Recordemos cambiar el nombre del botón
    estadoActual = digitalRead(pinBoton);
    // El if es para
    if (estadoAnterior == NO_PRESIONADO && estadoActual == PRESIONADO) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed = !estadoLed;
        digitalWrite(pinLed, estadoLed);
        delay(20);
    }

    estadoAnterior = estadoActual;

}