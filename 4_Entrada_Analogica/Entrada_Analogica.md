<h1>Entrada analógica con Arduino</h1>

- [Objetivo](#objetivo)
- [Práctica base: conversión A/D directa de 10 bits](#práctica-base-conversión-ad-directa-de-10-bits)
	- [Archivo de la práctica base](#archivo-de-la-práctica-base)
	- [Conexión sugerida](#conexión-sugerida)
	- [Algoritmo](#algoritmo)
	- [Funciones necesarias](#funciones-necesarias)
	- [¿Cómo leer bit por bit?](#cómo-leer-bit-por-bit)
- [Práctica adicional 1: barra creciente/decreciente con 10 LEDs](#práctica-adicional-1-barra-crecientedecreciente-con-10-leds)
	- [Algoritmo](#algoritmo-1)
	- [Funciones necesarias](#funciones-necesarias-1)
	- [¿Cómo funciona `map()`?](#cómo-funciona-map)
- [Práctica adicional 2: potenciómetro controlando PWM en D3](#práctica-adicional-2-potenciómetro-controlando-pwm-en-d3)
	- [Algoritmo](#algoritmo-2)
	- [Funciones necesarias](#funciones-necesarias-2)

## Objetivo

Leer una señal analógica desde `A0` y transformarla en salidas digitales o PWM para entender cómo Arduino pasa de una magnitud analógica a una representación digital útil.

## Práctica base: conversión A/D directa de 10 bits

Usa 10 LED en los pines `4` a `13` para mostrar directamente el resultado de la lectura analógica en formato binario de 10 bits (sin reducir a 8 bits).

### Archivo de la práctica base

- Código Arduino base: [Entrada_Analogica.ino](Entrada_Analogica.ino)

### Conexión sugerida

- Potenciómetro: extremos a `5V` y `GND`, cursor a `A0`.
- LEDs: uno por cada pin digital del `4` al `13` (cada LED con su resistencia).

### Algoritmo

```text
INICIO
	Definir arreglo con pines de LEDs [4..13]

	setup:
		Configurar cada pin del arreglo como salida

	loop:
		Leer A0 (valor ADC de 0 a 1023)
		Para cada bit de 0 a 9:
			Obtener estado del bit (0 o 1)
			Escribir ese estado en el LED correspondiente
FIN
```

### Funciones necesarias

- `analogRead(A0)`: lee el ADC del pin analógico (`0` a `1023`).
- `pinMode(pin, OUTPUT)`: define cada pin de LED como salida.
- Operaciones de bits (`>>`, `&`): extraen cada uno de los 10 bits de la lectura.
- `digitalWrite(pin, estado)`: enciende/apaga cada LED según el bit.

### ¿Cómo leer bit por bit?

La idea es que cada LED representa una posición binaria del número leído:

- LED 0 → bit 0 (menos significativo)
- LED 1 → bit 1
- ...
- LED 9 → bit 9 (más significativo)

En el ciclo `for`, se hace esto para cada posición:

1. **Mover** el número hacia la derecha `bit` posiciones con `>>`.
2. **Aislar** solo el último bit con `& 0x01`.
3. Ese resultado (`0` o `1`) se envía al LED.

Algoritmo mental rápido:

```text
Para bit = 0 hasta 9:
	estadoBit = (lecturaADC >> bit) & 1
	escribir estadoBit en el LED[bit]
```

Ejemplo corto con `lecturaADC = 13`:

- `13` en binario es `0000001101`
- Para `bit = 0`: `(13 >> 0) & 1 = 1` → LED 0 encendido
- Para `bit = 1`: `(13 >> 1) & 1 = 0` → LED 1 apagado
- Para `bit = 2`: `(13 >> 2) & 1 = 1` → LED 2 encendido
- Para `bit = 3`: `(13 >> 3) & 1 = 1` → LED 3 encendido

Con ese patrón, los LEDs muestran el número en binario sin convertirlo manualmente.

## Práctica adicional 1: barra creciente/decreciente con 10 LEDs

Aquí no se muestra el número en binario, sino una barra de nivel: al subir el potenciómetro se encienden más LEDs y al bajar se apagan.

### Algoritmo

```text
INICIO
	Definir arreglo con pines de LEDs [4..13]

	setup:
		Configurar los 10 pines como salida

	loop:
		Leer A0 (0 a 1023)
		Convertir lectura a cantidad de LEDs encendidos (0 a 10)
		Para cada LED (de 0 a 9):
			Si índice < cantidadEncendida -> encender
			Si no -> apagar
FIN
```

### Funciones necesarias

- `analogRead()`: lectura del potenciómetro.
- `map()`: ajuste del rango de entrada (`0–1023`) a salida (`0–10`).
- `for`: recorre los 10 LEDs para encender o apagar según el nivel.
- `digitalWrite()`: controla cada LED de la barra.

### ¿Cómo funciona `map()`?

`map()` convierte linealmente un valor de un rango a otro.  
Forma general:

$$
y = (x - in_{min}) \cdot \frac{out_{max} - out_{min}}{in_{max} - in_{min}} + out_{min}
$$

Ejemplo típico en Arduino:

- Entrada ADC: `0` a `1023` (10 bits o 1024 estados incluyendo apagado)
- Salida PWM: `0` a `10` (11 estados incluyendo apagado)

Entonces, cuando el potenciómetro está a la mitad, la salida aproximada también queda cerca de la mitad del nuevo rango.

> Nota: `map()` no limita automáticamente valores fuera de rango. Si hace falta limitar, se puede usar `constrain()`.

## Práctica adicional 2: potenciómetro controlando PWM en D3

Con el mismo potenciómetro en `A0`, controla también el brillo de un LED conectado al pin PWM `D3`.

### Algoritmo

```text
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

### Funciones necesarias

- `analogRead(A0)`: lectura analógica del potenciómetro.
- `map()`: conversión de `0–1023` a `0–255`.
- `analogWrite(3, valorPWM)`: aplica el ciclo de trabajo al LED.
- `delay(ms)`: suaviza lecturas rápidas y evita parpadeo visual brusco.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Salida "analógica" (PWM)](../3_Salida_PWM/Salida_PWM.md) | [Volver al índice](../README.md) | [Amplificadores operacionales básicos](../5_OpAmps_Basicos/OpAmps_Basicos.md) |