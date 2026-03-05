<h1>Entrada digital con Arduino</h1>

- [¿Qué es una entrada digital?](#qué-es-una-entrada-digital)
- [Práctica: botón en pin 2 y LED en pin 3](#práctica-botón-en-pin-2-y-led-en-pin-3)
  - [Archivo de la práctica](#archivo-de-la-práctica)
  - [¿Qué hace el código?](#qué-hace-el-código)
- [Ejercicios adicionales](#ejercicios-adicionales)
  - [1. Haz que el LED se encienda al presionar el botón y se mantenga prendido. Luego que se apague al volver a presionarlo.](#1-haz-que-el-led-se-encienda-al-presionar-el-botón-y-se-mantenga-prendido-luego-que-se-apague-al-volver-a-presionarlo)
  - [2. Añade otro botón en el pin digital `4` y un LED en el pin digital `5`.](#2-añade-otro-botón-en-el-pin-digital-4-y-un-led-en-el-pin-digital-5)

## ¿Qué es una entrada digital?

Una entrada digital permite leer dos estados en un pin de Arduino:

- `HIGH` (1 lógico)
- `LOW` (0 lógico)

Con un botón, estos dos niveles se usan para detectar si está **presionado** o **suelto**.

## Práctica: botón en pin 2 y LED en pin 3

En esta práctica se lee un botón en el pin `2` y se controla un LED en el pin `3`.

- Al pulsar el botón, el LED se enciende.
- Al soltarlo, el LED se apaga.

### Archivo de la práctica

- Código Arduino: [Entrada_Digital.ino](Entrada_Digital.ino)

### ¿Qué hace el código?

1. Configura el botón como `INPUT_PULLUP` en el pin `2`, lo que permite usar el botón sin necesidad de una resistencia externa.
2. Configura el LED como `OUTPUT` en el pin `3`.
3. Lee el estado del botón con `digitalRead()`.
4. Si el botón está pulsado (`LOW` con pull-up), enciende el LED.
5. Si no está pulsado, apaga el LED.

```cpp
#include <Arduino.h>

byte pinBoton = 2;
byte pinLed = 3;

// La variable estadoBoton se declara como bool 
// porque solo necesitamos saber si el botón 
// está presionado o no.
bool estadoBoton;

// Para no confundirnos con los valores de HIGH y LOW,
// definimos constantes para representar el estado del botón. 
const bool PRESIONADO = LOW; // El botón está presionado cuando el pin está en LOW
const bool NO_PRESIONADO = HIGH; // El botón no está presionado cuando el pin está en HIGH


void setup() {
	pinMode(pinBoton, INPUT_PULLUP);
	pinMode(pinLed, OUTPUT);
}

void loop() {
	estadoBoton = digitalRead(pinBoton);

    // La lógica del botón es inversa porque se usa INPUT_PULLUP,
    // el pin se mantiene en HIGH (5v) cuando el botón no está presionado
	if (estadoBoton == PRESIONADO) {
		digitalWrite(pinLed, HIGH);
	} else {
		digitalWrite(pinLed, LOW);
	}
}
```

## Ejercicios adicionales

### 1. Haz que el LED se encienda al presionar el botón y se mantenga prendido. Luego que se apague al volver a presionarlo.

Para eso, podemos guardar una variable del estado actual del LED y del estado anterior. Para eso, inicializamos las variables arriba del `void setup()`:

```cpp
bool estadoAnterior = HIGH;
```

También puedes cambiar 
```cpp
bool estadoBoton;
```
por

```cpp
bool estadoActual;
```
Además de añadir una variable para el LED que comience en el estado LOW
```cpp
bool estadoLed = LOW
```
Y luego dentro del `void loop()` ponemos un condicional `if` para poder detectar el botón
```cpp
    // Recordemos cambiar el nombre del botón
    estadoActual = digitalRead(pinBoton);
    // El if es para
    if (estadoAnterior == NO_PRESIONADO && estadoActual == PRESIONADO) {
        // usar ! en estadoLed sirve para intercambiar
        // entre HIGH y LOW 
        estadoLed = !estadoLed;
        digitalWrite(pinLed, estadoLed);
        delay(20);
    }

    estadoAnterior = estadoActual;
}
```



### 2. Añade otro botón en el pin digital `4` y un LED en el pin digital `5`.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Salida digital](../1_Salida_Digital/Salida_Digital.md) | [Volver al índice](../README.md#prácticas-básicas) | [Salida "analógica" (PWM)](../3_Salida_PWM/Salida_PWM.md) |