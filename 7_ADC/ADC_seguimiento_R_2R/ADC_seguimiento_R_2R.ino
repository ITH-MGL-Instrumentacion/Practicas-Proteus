#include <Arduino.h>
#include <avr/io.h>

// ---------------------------------------------------------
// ADC por seguimiento con DAC R-2R de 8 bits (versión mejorada)
//
// - PORTD (D0 a D7) entrega el valor digital al DAC R-2R.
// - D8 recibe la salida del comparador.
// - D9 genera una señal PWM para controlar un LED.
// ---------------------------------------------------------

const byte pinComparador = 8;
const byte pinLedPwm = 9;

#ifndef DDRD
#define DDRD _SFR_IO8(0x0A)
#endif

#ifndef PORTD
#define PORTD _SFR_IO8(0x0B)
#endif

byte valorDAC = 0;

// Ajusta según la lógica de tu comparador
const byte comparadorMayor = LOW;

// Pequeña banda muerta para evitar oscilaciones
const byte histeresis = 1;

void actualizarSalidas() {
  PORTD = valorDAC;
  analogWrite(pinLedPwm, valorDAC);
}

void setup() {
  DDRD = 0xFF; // Puerto D como salida
  pinMode(pinComparador, INPUT);
  pinMode(pinLedPwm, OUTPUT);

  actualizarSalidas();
}

void loop() {
  byte salidaComparador = digitalRead(pinComparador);

  if (salidaComparador == comparadorMayor) {
    if (valorDAC < 255 - histeresis) {
      valorDAC++;
    }
  } else {
    if (valorDAC > histeresis) {
      valorDAC--;
    }
  }

  actualizarSalidas();

  // Se redujo el tiempo de espera para mayor rapidez
  delayMicroseconds(10);
}
