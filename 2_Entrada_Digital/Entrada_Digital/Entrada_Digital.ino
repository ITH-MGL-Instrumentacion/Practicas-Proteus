#include <Arduino.h>

// ====================================================================
// CONFIGURACIÓN DE PINES
// ====================================================================
// Componentes originales (Ejercicio 1)
byte pinBoton = 2;
byte pinLed = 3;

// Nuevos componentes añadidos (Ejercicio 2)
byte pinBoton2 = 4;
byte pinLed2 = 5;

// ====================================================================
// CONSTANTES Y VARIABLES
// ====================================================================
// Constantes para la lógica inversa debido al uso de INPUT_PULLUP
const bool PRESIONADO = LOW; 
const bool NO_PRESIONADO = HIGH; 

// Variables para el EJERCICIO 1 (Botón como Interruptor)
bool estadoActual;
bool estadoAnterior = HIGH; // Se inicializa en HIGH porque el pull-up mantiene el pin en alto
bool estadoLed = LOW;       // El LED iniciará apagado

// Variables para el EJERCICIO 2 (Segundo Botón - Lógica básica)
bool estadoBoton2;


void setup() {
    // Configuración de los componentes del Ejercicio 1
    pinMode(pinBoton, INPUT_PULLUP);
    pinMode(pinLed, OUTPUT);
    
    // Configuración de los componentes del Ejercicio 2
    pinMode(pinBoton2, INPUT_PULLUP);
    pinMode(pinLed2, OUTPUT);
}


void loop() {
    // ================================================================
    // EJERCICIO 1: Lógica de Interruptor (Toggle) para Botón 1 y LED 1
    // ================================================================
    
    // 1. Leemos el estado actual del primer botón
    estadoActual = digitalRead(pinBoton);

    // 2. Detectamos el "flanco de bajada" (el momento exacto en que se presiona)
    // Es decir: antes NO estaba presionado, pero AHORA SÍ lo está.
    if (estadoAnterior == NO_PRESIONADO && estadoActual == PRESIONADO) {
        
        estadoLed = !estadoLed; // El signo '!' invierte el valor (si era LOW cambia a HIGH, si era HIGH cambia a LOW)
        digitalWrite(pinLed, estadoLed); // Aplicamos el nuevo estado al LED 1
        
        delay(20); // Pequeña pausa (Antirrebote o Debounce) para evitar lecturas falsas por el ruido mecánico del botón
    }
    
    // 3. Guardamos el estado actual para que sea el "anterior" en la siguiente vuelta del ciclo
    estadoAnterior = estadoActual;


    // ================================================================
    // EJERCICIO 2: Lógica básica (Momentánea) para Botón 2 y LED 2
    // ================================================================
    
    // 1. Leemos el estado del segundo botón (pin 4)
    estadoBoton2 = digitalRead(pinBoton2);

    // 2. Evaluamos su condición (Lógica original: enciende solo mientras se mantenga presionado)
    if (estadoBoton2 == PRESIONADO) {
        digitalWrite(pinLed2, HIGH); // Enciende el LED 2 (pin 5)
    } else {
        digitalWrite(pinLed2, LOW);  // Apaga el LED 2 (pin 5)
    }
}
