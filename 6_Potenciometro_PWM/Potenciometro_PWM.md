# Práctica adicional 2: potenciómetro controlando PWM en D3

## Objetivo
Usar el mismo potenciómetro conectado a A0 para:
1. Controlar el brillo de un LED en el pin PWM D3.
2. Encender una barra de 10 LEDs (pines 4 a 13) proporcionalmente al giro del potenciómetro.

### Archivos de la práctica

- Código Arduino: [Potenciometro_PWM.ino](Potenciometro_PWM.ino)

## Circuito
- Conectar un potenciómetro de 10kΩ entre 5V, GND y A0 del Arduino.
- Conectar un LED con una resistencia de 220Ω en serie al pin D3 (PWM).
- Conectar 10 LEDs con resistencias de 220Ω en serie a los pines D4 a D13.

## ¿Qué hace el código?

El código lee el valor del potenciómetro en A0, lo mapea para controlar el brillo de un LED vía PWM en D3, y enciende una cantidad proporcional de LEDs en los pines 4 a 13.

```cpp
#include <Arduino.h>

const byte pwmPin = 3;          // LED controlado por PWM en D3
const byte potPin = A0;         // Entrada analógica del potenciómetro
const byte ledPins[] = {4,5,6,7,8,9,10,11,12,13}; // LEDs en los pines digitales 4..13
const byte numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void setup() {
    pinMode(pwmPin, OUTPUT);
    for (byte i = 0; i < numLeds; ++i) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

void loop() {
    // leer el potenciómetro (0..1023)
    int lectura = analogRead(potPin);

    // convertir a ciclo de trabajo PWM (0..255)
    byte valorPWM = map(lectura, 0, 1023, 0, 255);
    analogWrite(pwmPin, valorPWM);

    // calcular cuántos LEDs encender
    byte ledsEncendidos = map(lectura, 0, 1023, 0, numLeds);
    for (byte i = 0; i < numLeds; ++i) {
        if (i < ledsEncendidos) {
            digitalWrite(ledPins[i], HIGH);
        } else {
            digitalWrite(ledPins[i], LOW);
        }
    }

    // pequeña pausa para evitar parpadeo brusco
    delay(50);
}
```

## Algoritmo
```
INICIO
    Definir pin LED PWM = 3
    Definir arreglo con pines de LEDs [4..13]

    setup:
        Configurar pin PWM como salida
        Configurar los 10 pines como salida

    loop:
        Leer A0 (0 a 1023)
        Convertir lectura a PWM (0 a 255)
        Escribir PWM en D3
        Convertir lectura a cantidad de LEDs encendidos (0 a 10)
        Para cada LED (de 0 a 9):
            Si índice < cantidadEncendida -> encender
            Si no -> apagar
        Esperar un tiempo corto para estabilidad visual
FIN
```

### Funciones usadas
- `analogRead(A0)` – lectura analógica del potenciómetro.
- `map()` – conversión de 0–1023 a 0–255 o a 0–10.
- `analogWrite(3, valorPWM)` – aplica el ciclo de trabajo al LED PWM.
- `digitalWrite()` – controla los LEDs individuales.
- `delay(ms)` – suaviza lecturas rápidas y evita parpadeo visual brusco.

## Observaciones
- El valor de `map()` para los LEDs redondea hacia abajo; con la lectura máxima se encienden los 10 LEDs.
- El retardo de 50 ms es suficiente para una percepción estable sin afectar la respuesta.
