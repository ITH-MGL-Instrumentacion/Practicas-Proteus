<h1>Salida digital con Arduino</h1>

- [¿Qué es una salida digital?](#qué-es-una-salida-digital)
- [Práctica: LED parpadeando en el pin 3](#práctica-led-parpadeando-en-el-pin-3)
  - [Archivos de la práctica](#archivos-de-la-práctica)
  - [¿Qué hace el código?](#qué-hace-el-código)
- [Ejercicios adicionales](#ejercicios-adicionales)

## ¿Qué es una salida digital?

Una salida digital en Arduino permite poner un pin en dos estados:

- `HIGH` (nivel alto, típicamente 5V en Arduino UNO)
- `LOW` (nivel bajo, 0V)

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

## Ejercicios adicionales

1. Cambia el tiempo de encendido y apagado para modificar la velocidad de parpadeo.
2. Añade otro LED en el pin digital `5` y haz que parpadee con un patrón distinto al del pin `3`.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Formato de los proyectos en Visual Studio Code](../0_Introduccion/Formato_proyectos.md) | [Volver al índice](../README.md#prácticas-básicas) | [Entrada digital](../2_Entrada_Digital/Entrada_Digital.md) |