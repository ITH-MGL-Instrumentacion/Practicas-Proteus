#include <Arduino.h>

byte pinBoton = 2;
byte pinLed = 3;
byte estadoBoton;

void setup() {
	pinMode(pinBoton, INPUT_PULLUP);
	pinMode(pinLed, OUTPUT);
}

void loop() {
	estadoBoton = digitalRead(pinBoton);

	if (estadoBoton == LOW) {
		digitalWrite(pinLed, HIGH);
	} else {
		digitalWrite(pinLed, LOW);
	}
}
