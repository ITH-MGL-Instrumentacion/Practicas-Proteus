/*
  Práctica: LED parpadeando en el pin 3
  Descripción: Enciende y apaga un LED conectado al pin digital 3
               usando tiempos de espera en milisegundos.
*/

// Definición de variables globales
const byte ledPin = 3;          // Pin digital donde se conecta el LED
int tiempoEncendido = 500;     // Tiempo que permanece encendido (ms)
uint16_t tiempoApagado = 500;   // Tiempo que permanece apagado (ms)

void setup() {
  // Configura el pin digital 3 como salida
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);   // Pone el pin en estado ALTO (5V), enciende el LED
  delay(tiempoEncendido);       // Pausa el programa por 500 milisegundos

  digitalWrite(ledPin, LOW);    // Pone el pin en estado BAJO (0V), apaga el LED
  delay(tiempoApagado);        // Pausa el programa por 500 milisegundos
}
