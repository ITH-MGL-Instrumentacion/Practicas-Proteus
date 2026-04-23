#include <Arduino.h>
#include <avr/io.h>
#include <math.h>

// ---------------------------------------------------------
// DAC por suma ponderada usando PORTD completo (D0 a D7)
//
// - A0 controla la frecuencia de la senoide.
// - A1 controla la amplitud.
// - Los potenciómetros se leen cada 100 ms con millis().
// - La salida de 8 bits se actualiza con micros().
//
// Nota importante:
// PORTD controla los pines digitales 0 a 7 del Arduino UNO.
// Esto incluye D0 y D1, por lo que no conviene usar Serial
// al mismo tiempo que este programa está corriendo.
// ---------------------------------------------------------

const byte pinPotFrecuencia = A0;
const byte pinPotAmplitud = A1;

// Si IntelliSense no resuelve los registros AVR, se declaran aquí
// únicamente para mantener legible el código en VS Code.
#ifndef DDRD
#define DDRD _SFR_IO8(0x0A)
#endif

#ifndef PORTD
#define PORTD _SFR_IO8(0x0B)
#endif

const uint16_t tablaTam = 256;
byte tablaSeno[tablaTam];

// Rango razonable para una demostración con DAC de 8 bits.
const float FRECUENCIA_MAX_HZ = 100.0;
float frecuenciaHz = 5.0;
float amplitud = 127.0;

unsigned long ultimoControlMs = 0;
unsigned long ultimoPasoUs = 0;
unsigned long periodoMuestraUs = 1000;

uint16_t indiceTabla = 0;

void setup() {
	// Configura D0 a D7 como salidas para usar PORTD completo.
	DDRD = 0xFF;

	// Precalcular la tabla seno vale la pena porque evita llamar
	// sin() en cada iteración del lazo rápido.
	for (uint16_t i = 0; i < tablaTam; i++) {
		float angulo = 2.0 * PI * i / tablaTam;
		float senoNormalizado = (sin(angulo) + 1.0) * 0.5;
		tablaSeno[i] = (byte)(senoNormalizado * 255.0);
	}

	// Lectura inicial de potenciómetros.
	int lecturaFreq = analogRead(pinPotFrecuencia);
	int lecturaAmp = analogRead(pinPotAmplitud);

	frecuenciaHz = 1.0 + ((FRECUENCIA_MAX_HZ - 1.0) * lecturaFreq / 1023.0);
	amplitud = 127.0 * lecturaAmp / 1023.0;

	periodoMuestraUs = (unsigned long)(1000000.0 / (frecuenciaHz * tablaTam));
	ultimoControlMs = millis();
	ultimoPasoUs = micros();
}

void loop() {
	unsigned long ahoraMs = millis();
	unsigned long ahoraUs = micros();

	// Actualizar controles cada 100 ms.
	if (ahoraMs - ultimoControlMs >= 100) {
		ultimoControlMs = ahoraMs;

		int lecturaFreq = analogRead(pinPotFrecuencia);
		int lecturaAmp = analogRead(pinPotAmplitud);

		// Frecuencia de 1 a FRECUENCIA_MAX_HZ Hz.
		frecuenciaHz = 1.0 + ((FRECUENCIA_MAX_HZ - 1.0) * lecturaFreq / 1023.0);

		// Amplitud de 0 a 127 para no recortar al centrar en 127.
		amplitud = 127.0 * lecturaAmp / 1023.0;

		periodoMuestraUs = (unsigned long)(1000000.0 / (frecuenciaHz * tablaTam));
		if (periodoMuestraUs == 0) {
			periodoMuestraUs = 1;
		}
	}

	// Actualizar la salida DAC según el periodo calculado.
	if (ahoraUs - ultimoPasoUs >= periodoMuestraUs) {
		ultimoPasoUs += periodoMuestraUs;

		// La tabla va de 0 a 255 y se recentra respecto al punto medio.
		int16_t muestraCentrada = (int16_t)tablaSeno[indiceTabla] - 127;
		int16_t muestraEscalada = (int16_t)(muestraCentrada * amplitud / 127.0);
		int16_t salida = 127 + muestraEscalada;
        
		if (salida < 0) {
			salida = 0;
		}
		if (salida > 255) {
			salida = 255;
		}

		PORTD = (byte)salida;
		indiceTabla = (indiceTabla + 1) % tablaTam;
	}
}
