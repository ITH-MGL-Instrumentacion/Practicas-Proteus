#include <Arduino.h>

byte pinLed = 3;

// 50% de 255 ≈ 127.5, se usa 128
byte valorPWM = 128;

void setup() {
	pinMode(3, OUTPUT);
}

void loop() {
	analogWrite(3, 128);
}
