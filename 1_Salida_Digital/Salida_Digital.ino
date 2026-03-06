#include <Arduino.h>

// byte usa 8 bits (0 a 255)
byte ledPin1 = 3;
byte ledPin2 = 5;
// int usa 16 bits (-32,768 a 32,767)
int tiempoEncendido = 200;  //milisegundos
// uint16_t (unsigned int) usa 16 bits (0 a 65,535)
uint16_t tiempoApagado = 800;    //milisegundos
// pueden usarse otros tipos de datos dependiendo 
// de las necesidades del programa, como:
// long, float, double, etc.

void setup() {
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
}

void loop() {
    // HIGH es el valor lógico 1/true o 5v,
    // LOW es 0/false o 0v
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    delay(300); 
    // delay() recibe un valor en milisegundos
    // el programa se detiene por el tiempo indicado
    

    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(500);
}