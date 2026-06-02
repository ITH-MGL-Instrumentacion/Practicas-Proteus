# Control PID de la hielera

Esta práctica compara tres estrategias de control de temperatura sobre la misma hielera de poliestireno: control **todo-o-nada (On/Off)**, control con **histéresis** y control **PID**. Con un botón en `D2` se cicla entre los tres modos sin recargar el programa, y una pantalla LCD I2C muestra la temperatura, la humedad y el modo activo en tiempo real.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Modelado de la hielera](../11_Modelado_hielera/11_Modelado_hielera.md) | [Volver al índice](../README.md) | — |

<h2>Índice</h2>

- [Objetivo](#objetivo)
- [1. Hardware de la práctica](#1-hardware-de-la-práctica)
- [2. Control todo-o-nada (On/Off)](#2-control-todo-o-nada-onoff)
- [3. Control con histéresis](#3-control-con-histéresis)
- [4. Control PID discreto](#4-control-pid-discreto)
  - [Parámetros del controlador](#parámetros-del-controlador)
  - [Forma del algoritmo](#forma-del-algoritmo)
  - [Anti-windup](#anti-windup)
- [5. Descripción del código](#5-descripción-del-código)
- [6. Comparación entre modos](#6-comparación-entre-modos)
- [7. Actividades propuestas](#7-actividades-propuestas)

## Objetivo

Comparar tres estrategias de control de temperatura (On/Off, histéresis y PID) sobre la hielera de poliestireno. Al final de la práctica el alumno debe ser capaz de identificar las diferencias en oscilación residual, tiempo de establecimiento y esfuerzo de control entre los tres modos.

## 1. Hardware de la práctica

| Componente | Conexión | Función |
|---|---|---|
| LM35 | `A0` | Sensor de temperatura para el lazo de control |
| MOSFET IRLZ44N | `D3` (PWM) | Accionamiento del calefactor |
| Ventilador 12 V | `D5` (PWM) | Circulación de aire (corre siempre) |
| DHT11 | `D4` | Temperatura y humedad ambiental (solo lectura) |
| LCD I2C 16×2 | `A4`/`A5` | Visualización de temperatura, humedad y modo |
| Botón | `D2` → GND | Cicla entre modos: On/Off → Histéresis → PID |

> La temperatura que el controlador usa internamente es la del **LM35** (sensor del sistema térmico). El DHT11 solo sirve para mostrar la humedad en la LCD.

## 2. Control todo-o-nada (On/Off)

Esta es la estrategia más simple. El calefactor se enciende al máximo (`PWM = 255`) cuando la temperatura está por debajo del setpoint, y se apaga completamente (`PWM = 0`) cuando la supera:

$$
u_k = \begin{cases} 255 & \text{si } T_k < T_{ref} \\ 0 & \text{si } T_k \geq T_{ref} \end{cases}
$$

**Consecuencia:** el sistema nunca se estabiliza exactamente en el setpoint; la temperatura oscila continuamente alrededor de él. La frecuencia y amplitud de esa oscilación dependen de la inercia térmica del sistema.

## 3. Control con histéresis

Agrega una banda muerta de ± `HISTERESIS_C` alrededor del setpoint para reducir la frecuencia de conmutación. El calefactor solo cambia de estado cuando la temperatura cruza uno de los dos umbrales:

$$
\text{Enciende si } T_k < T_{ref} - \Delta_h \qquad \text{Apaga si } T_k > T_{ref} + \Delta_h
$$

Donde $\Delta_h$ = `HISTERESIS_C = 1.0 °C` (configurable en el código).

**Ventaja frente a On/Off puro:** la oscilación tiene menor frecuencia y el calefactor no conmuta con cada muestra. **Desventaja:** el error estacionario es proporcional a $\Delta_h$; a mayor banda, más oscilación permitida.

## 4. Control PID discreto

### Parámetros del controlador

Los parámetros se obtienen ejecutando todas las celdas de [`sintonizador.ipynb`](../11_Modelado_hielera/sintonizador.ipynb) con el CSV de la respuesta al escalón. La tabla de resultados del notebook tiene la forma:

| Parámetro | Descripción | Cómo se copia al `.ino` |
|---|---|---|
| $K_p$ | Ganancia proporcional | `const float KP` |
| $K_i$ | Ganancia integral | `const float KI` |
| $K_d$ | Ganancia derivativa | `const float KD` |

El setpoint se ajusta con la constante `SETPOINT_C` en el código.

### Forma del algoritmo

El código usa la **forma paralela discreta con diferencia hacia atrás**. En cada período de muestreo $T_s$:

$$
e_k = T_{ref} - T_k
$$

$$
u_k = K_p\, e_k + K_i \sum_{j=0}^{k} e_j\, T_s + K_d\, \frac{e_k - e_{k-1}}{T_s}
$$

La salida $u_k$ se limita al rango `[0, 255]` antes de enviarse al MOSFET con `analogWrite`.

### Anti-windup

Cuando la salida satura (llega a 0 o 255), el integrador deja de acumular error para evitar que el término integral crezca sin límite y retrase la recuperación al volver dentro del rango. Esto se implementa congelando la actualización del acumulador integral en el instante de saturación.

## 5. Descripción del código

El archivo [`12_Control_hielera.ino`](12_Control_hielera.ino) hace lo siguiente:

1. Inicializa la LCD I2C (dirección `0x27`, cambiar a `0x3F` si no enciende) y el DHT11.
2. Configura el botón en `D2` con pull-up interno; cada pulsación cicla el modo activo.
3. En cada período `TS_MS = 500 ms` lee el LM35 y el DHT11, calcula la salida según el modo y escribe el PWM.
4. Cada segundo refresca la LCD con la temperatura, la humedad y el nombre del modo.
5. Al cambiar de modo resetea el integrador y apaga el calefactor para partir de estado limpio.

Parámetros a ajustar en las constantes del `.ino`:

```cpp
const float SETPOINT_C   = 37.7;    // Temperatura objetivo (°C)
const float HISTERESIS_C = 1.0;     // Semibanda del modo histéresis (°C)
const float KP           = 13.9166; // Copiar del sintonizador
const float KI           = 0.253325;
const float KD           = 150.8610;
const float TS_MS        = 500.0;   // Período de muestreo (ms)
```

## 6. Comparación entre modos

| Característica | On/Off | Histéresis | PID |
|---|---|---|---|
| Error estacionario | Alto (oscila alrededor de SP) | Proporcional a $\Delta_h$ | Nulo (con término integral) |
| Frecuencia de conmutación | Muy alta | Reducida por la banda muerta | Continua (PWM analógico) |
| Sobreimpulso | Presente | Presente | Controlable con $K_d$ |
| Rechazo a perturbaciones | Lento | Lento | Rápido |
| Complejidad de ajuste | Ninguna | Un parámetro ($\Delta_h$) | Tres parámetros ($K_p$, $K_i$, $K_d$) |
| Desgaste del actuador | Alto (muchas conmutaciones) | Menor | Mínimo (salida continua) |

> **Observación en la práctica:** el modo On/Off genera la mayor oscilación visible en la hielera. El modo histéresis la reduce pero no la elimina. Solo el PID logra llevar y mantener la temperatura exactamente en el setpoint una vez alcanzado el régimen permanente.

## 7. Actividades propuestas

1. Carga el programa y deja el sistema arrancar en modo **On/Off**. Espera a que la temperatura se estabilice y registra la amplitud de la oscilación alrededor del setpoint.
2. Pulsa el botón para cambiar a **Histéresis**. Espera al régimen permanente y mide la nueva amplitud de oscilación. Compara con el paso anterior.
3. Pulsa el botón para cambiar a **PID**. Observa el transitorio y mide el tiempo de establecimiento y el sobreimpulso.
4. Copia los valores de $K_p$, $K_i$, $K_d$ del `sintonizador.ipynb` a las constantes del `.ino`, recarga y repite el paso 3.
5. Con el PID activo, aplica una perturbación (abre la tapa de la hielera 10 s) y registra cuánto tarda en recuperarse.
6. Repite la perturbación en modo histéresis y compara el tiempo de recuperación.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Modelado de la hielera](../11_Modelado_hielera/11_Modelado_hielera.md) | [Volver al índice](../README.md) | — |
