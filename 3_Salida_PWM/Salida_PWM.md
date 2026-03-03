<h1>Salida PWM con Arduino</h1>

- [¿Qué es una salida PWM?](#qué-es-una-salida-pwm)
- [Práctica: LED con PWM fijo al 50% en el pin 3](#práctica-led-con-pwm-fijo-al-50-en-el-pin-3)
  - [Archivo de la práctica](#archivo-de-la-práctica)
  - [¿Qué hace el código?](#qué-hace-el-código)
  - [Funciones clave que debes entender](#funciones-clave-que-debes-entender)
- [Ejercicios adicionales](#ejercicios-adicionales)
  - [1. Selecciona 5 valores PWM: 0%, 25%, 50%, 75% y 100% y haz que vayan cambiando en un intervalo de tiempo.](#1-selecciona-5-valores-pwm-0-25-50-75-y-100-y-haz-que-vayan-cambiando-en-un-intervalo-de-tiempo)
  - [2. Haz que ahora use una función seno con un periodo de 1 segundo para cambiar la iluminación del LED.](#2-haz-que-ahora-use-una-función-seno-con-un-periodo-de-1-segundo-para-cambiar-la-iluminación-del-led)

## ¿Qué es una salida PWM?

PWM significa **Pulse Width Modulation** (modulación por ancho de pulso). En Arduino, `analogWrite()` no entrega un voltaje analógico real, sino una señal digital que cambia rápido entre `HIGH` y `LOW`.

El brillo de un LED depende del **ciclo de trabajo** (duty cycle):

- `0%` → siempre apagado
- `50%` → mitad del tiempo encendido
- `100%` → siempre encendido

En Arduino UNO, `analogWrite(pin, valor)` usa un valor de `0` a `255`.

## Práctica: LED con PWM fijo al 50% en el pin 3

En esta práctica se configura un LED en el pin PWM `3` y se deja con un duty cycle fijo del `50%`.

### Archivo de la práctica

- Código Arduino: [Salida_PWM.ino](Salida_PWM.ino)

### ¿Qué hace el código?

1. Configura el pin `3` como salida.
2. Calcula el valor PWM para `50%` (`128` aprox. en escala de `0` a `255`).
3. Escribe ese valor continuamente con `analogWrite()`.

```cpp
#include <Arduino.h>

byte pinLed = 3;

// 50% de 255 ≈ 127.5, se usa 128
byte pwm50 = 128;

void setup() {
    pinMode(pinLed, OUTPUT);
}

void loop() {
    analogWrite(pinLed, pwm50);
}
```

### Funciones clave que debes entender

- `pinMode(pin, OUTPUT)`: define el pin como salida.
- `analogWrite(pin, valorPWM)`: genera PWM en pines compatibles (`3, 5, 6, 9, 10, 11` en UNO).
- Rango PWM en UNO: `0` a `255`.
    - `0` = apagado
    - `255` = encendido máximo

Conversión rápida de porcentaje a PWM:

$$
\mathrm{valor\ PWM} = \frac{porcentaje}{100} \times 255
$$

Ejemplo para 50%:

$$
\mathrm{valor\ PWM} = 0.5 \times 255 \approx 128
$$

## Ejercicios adicionales

### 1. Selecciona 5 valores PWM: 0%, 25%, 50%, 75% y 100% y haz que vayan cambiando en un intervalo de tiempo.

Puedes usar un arreglo con los valores equivalentes en 8 bits:

- `0%  -> 0`
- `25% -> 64`
- `50% -> 128`
- `75% -> 191`
- `100% -> 255`

```cpp
#include <Arduino.h>

byte pinLed = 3;
// Pon los valores
byte nivelesPWM[] = {valor1, valor2, valor3, valor4, valor5};
byte totalNiveles = 5;

void setup() {
    pinMode(pinLed, OUTPUT);
}

void loop() {
    for (byte i = 0; i < totalNiveles; i++) {
        // Recorre cada nivel de brillo del arreglo
        analogWrite(pinLed, nivelesPWM[i]);
        delay(tiempo);
    }
}
```

Este `for` recorre todos los valores de `nivelesPWM` y aplica cada brillo por `200 ms`.

Algoritmo sugerido:

```text
INICIO
    Definir arreglo con niveles PWM [0, 64, 128, 191, 255]
    Configurar pin LED como salida

    loop:
        Para cada nivel del arreglo:
            Escribir PWM en LED
            Esperar un tiempo fijo (ej. 200 ms)
FIN
```

### 2. Haz que ahora use una función seno con un periodo de 1 segundo para cambiar la iluminación del LED.

La función seno entrega valores de `-1` a `1`; se escalan a `0` a `255` para usarlos en PWM.  
Para un periodo de `1 s`, se usa $T = 1000$ ms.

```cpp
#include <Arduino.h>
#include <math.h>

byte pinLed = 3;
const float periodoMs = 1000.0;

void setup() {
    pinMode(pinLed, OUTPUT);
}

void loop() {
    unsigned long t = millis();

    // Ángulo en radianes para un periodo de 1 segundo
    float angulo = 2.0 * PI * (t % (unsigned long)periodoMs) / periodoMs;

    // Escalar de [-1, 1] a [0, 255]
    float senoNormalizado = (sin(angulo) + 1.0) * 0.5;
    byte valorPWM = (byte)(senoNormalizado * 255.0);

    analogWrite(pinLed, valorPWM);
    delay(5);
}
```

Algoritmo sugerido:

```text
INICIO
    Definir periodo T = 1000 ms
    Configurar pin LED como salida

    loop:
        Leer tiempo actual con millis()
        Convertir tiempo a ángulo de seno (0 a 2π)
        Evaluar seno (rango -1 a 1)
        Escalar a rango PWM (0 a 255)
        Escribir PWM al LED
FIN
```

Funciones extra importantes en este ejercicio:

- `millis()`: devuelve el tiempo transcurrido desde que inició el programa.
- `sin(x)`: calcula el seno del ángulo `x` (en radianes).
- `delay(ms)`: pausa breve para suavizar el cambio visual y reducir carga de CPU.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Entrada digital](../2_Entrada_Digital/Entrada_Digital.md) | [Volver al índice](../README.md#prácticas-básicas) | [Entrada analógica](../4_Entrada_Analogica/Entrada_Analogica.md) |