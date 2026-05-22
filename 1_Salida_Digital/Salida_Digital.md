<h1>Salida digital con Arduino</h1>

- [¿Qué es una salida digital?](#qué-es-una-salida-digital)
- [Práctica: LED parpadeando en el pin 3](#práctica-led-parpadeando-en-el-pin-3)
  - [Archivos de la práctica](#archivos-de-la-práctica)
  - [¿Qué hace el código?](#qué-hace-el-código)
- [Ejercicios adicionales](#ejercicios-adicionales)
  - [1. Cambia el tiempo de encendido y apagado para modificar la velocidad de parpadeo.](#1-cambia-el-tiempo-de-encendido-y-apagado-para-modificar-la-velocidad-de-parpadeo)
  - [2. Añade otro LED en el pin digital `5` y haz que parpadee con un patrón distinto al del pin `3`.](#2-añade-otro-led-en-el-pin-digital-5-y-haz-que-parpadee-con-un-patrón-distinto-al-del-pin-3)

## ¿Qué es una salida digital?

Una salida digital en Arduino permite poner un pin en dos estados:

- `HIGH` (nivel alto, típicamente 5 V en Arduino UNO)
- `LOW` (nivel bajo, 0 V)

Con esto se puede controlar dispositivos sencillos como LEDs, relevadores o señales lógicas para otros circuitos.

## Práctica: LED parpadeando en el pin 3

En esta práctica, el Arduino enciende y apaga un LED conectado al pin digital `3`, usando tiempos de espera en milisegundos.

### Archivos de la práctica

- Código Arduino: [Salida_Digital.ino](Salida_Digital.ino)
- Simulación Proteus: [Salida_Digital.pdsprj](Salida_Digital.pdsprj)

### ¿Qué hace el código?

1. Declara el pin del LED (`ledPin = 3`).
2. Configura ese pin como salida en `setup()` con `pinMode(..., OUTPUT)`.
3. En `loop()`, alterna:
     - `digitalWrite(ledPin, HIGH)` y espera `tiempoEncendido`.
     - `digitalWrite(ledPin, LOW)` y espera `tiempoApagado`.

Esto produce el parpadeo continuo del LED.

```cpp
byte ledPin = 3;
int tiempoEncendido = 500;
uint16_t tiempoApagado = 500;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(tiempoEncendido);

  digitalWrite(ledPin, LOW);
  delay(tiempoApagado);
}
```

## Ejercicios adicionales

### 1. Cambia el tiempo de encendido y apagado para modificar la velocidad de parpadeo.

Puedes lograrlo cambiando los valores de tiempo:

```cpp
int tiempoEncendido = 200;
uint16_t tiempoApagado = 800;
```

Con esto el LED estará menos tiempo encendido que apagado.

### 2. Añade otro LED en el pin digital `5` y haz que parpadee con un patrón distinto al del pin `3`.

Ejemplo base:

```cpp
byte ledPin1 = 3;
byte ledPin2 = 5;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(300);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(700);
}
```

| Anterior | Índice | Siguiente |
|---|---|---|
| [Formato de los proyectos en Visual Studio Code](../0_Introduccion/Formato_proyectos.md) | [Volver al índice](../README.md) | [Entrada digital](../2_Entrada_Digital/Entrada_Digital.md) |