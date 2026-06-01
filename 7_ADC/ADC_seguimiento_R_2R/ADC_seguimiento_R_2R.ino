#include <Arduino.h>
#include <avr/io.h>

// ---------------------------------------------------------
// ADC por seguimiento con DAC R-2R de 8 bits
//
// - PORTD (D0 a D7) entrega el valor digital al DAC R-2R.
// - D8 recibe la salida del comparador.
// - D9 genera una señal PWM para controlar un LED.
// - El potenciómetro se conecta a la entrada analógica del comparador.
//
// Funcionamiento:
// El programa ajusta el valor del DAC paso a paso hasta que el voltaje
// generado por PORTD se aproxime al voltaje del potenciómetro. El código
// resultante se usa como valor PWM para cambiar el brillo de un LED.
// ---------------------------------------------------------

const byte pinComparador = 8;
const byte pinLedPwm = 9;

// Si IntelliSense no resuelve los registros AVR, se declaran aquí
// únicamente para mantener legible el código en VS Code.
#ifndef DDRD
#define DDRD _SFR_IO8(0x0A)
#endif

#ifndef PORTD
#define PORTD _SFR_IO8(0x0B)
#endif

byte valorDAC = 0;

// Si la salida del comparador está invertida en tu circuito, cambia esta
// constante.
const byte comparadorMayor = LOW;

void setup() {
	DDRD = 0xFF;
	pinMode(pinComparador, INPUT);
	pinMode(pinLedPwm, OUTPUT);

	PORTD = valorDAC;
	analogWrite(pinLedPwm, valorDAC);
}

void loop() {
	byte salidaComparador = digitalRead(pinComparador);

	// Si el comparador indica que el voltaje del potenciómetro es mayor
	// que el del DAC, subimos un paso. En caso contrario, bajamos un paso.
	if (salidaComparador == comparadorMayor) {
		if (valorDAC < 255) {
			valorDAC++;
		}
	} else {
		if (valorDAC > 0) {
			valorDAC--;
		}
	}

	PORTD = valorDAC;
	analogWrite(pinLedPwm, valorDAC);

	// Pequeña espera para dar tiempo a estabilizar el comparador y el DAC.
	delayMicroseconds(200);
}
