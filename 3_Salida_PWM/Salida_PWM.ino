#include <Arduino.h>

byte pinLed = 3;
// Pon los valores
byte nivelesPWM[] = {0, 64, 128, 191, 255};
byte totalNiveles = 5;

void setup() 
{
    pinMode(pinLed, OUTPUT);
}

void loop() 
{
    for (byte i = 0; i < totalNiveles; i++) {
        // Recorre cada nivel de brillo del arreglo
        analogWrite(pinLed, nivelesPWM[i]);
        delay(1000);
    }
}