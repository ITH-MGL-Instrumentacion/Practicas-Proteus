#include <Arduino.h>

// byte usa 8 bits (0 a 255)
byte ledPin = 3;
// int usa 16 bits (-32,768 a 32,767)
int tiempoEncendido = 500;  //milisegundos
// uint16_t (unsigned int) usa 16 bits (0 a 65,535)
uint16_t tiempoApagado = 500;    //milisegundos
// pueden usarse otros tipos de datos dependiendo 
// de las necesidades del programa, como:
// long, float, double, etc.

void setup() {
    pinMode(ledPin, OUTPUT);
}

void loop() {
    // HIGH es el valor lógico 1/true o 5v,
    // LOW es 0/false o 0v
    digitalWrite(ledPin, HIGH);
    // delay() recibe un valor en milisegundos
    // el programa se detiene por el tiempo indicado
    delay(tiempoEncendido);

    digitalWrite(ledPin, LOW);
    delay(tiempoApagado);
}