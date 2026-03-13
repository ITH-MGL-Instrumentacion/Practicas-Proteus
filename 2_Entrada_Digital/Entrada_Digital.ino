#include <Arduino.h>

byte pinBoton1 = 2;
byte pinLed1   = 3;

byte pinBoton2 = 4;
byte pinLed2   = 5;

bool estadoAnterior1 = HIGH;
bool estadoActual1;
bool estadoLed1 = LOW;

bool estadoAnterior2 = HIGH;
bool estadoActual2;
bool estadoLed2 = LOW;

const bool PRESIONADO = LOW;
const bool NO_PRESIONADO = HIGH;

void setup() {
  pinMode(pinBoton1, INPUT_PULLUP);
  pinMode(pinLed1, OUTPUT);

  pinMode(pinBoton2, INPUT_PULLUP);
  pinMode(pinLed2, OUTPUT);
}

void loop() {
 
  estadoActual1 = digitalRead(pinBoton1);
  if (estadoAnterior1 == NO_PRESIONADO && estadoActual1 == PRESIONADO) {
    estadoLed1 = !estadoLed1;             
    digitalWrite(pinLed1, estadoLed1);
    delay(20);                            
  }
  estadoAnterior1 = estadoActual1;


  estadoActual2 = digitalRead(pinBoton2);
  if (estadoAnterior2 == NO_PRESIONADO && estadoActual2 == PRESIONADO) {
    estadoLed2 = !estadoLed2;             
    digitalWrite(pinLed2, estadoLed2);
    delay(20);                             // Anti-rebote
  }
  estadoAnterior2 = estadoActual2;
}

