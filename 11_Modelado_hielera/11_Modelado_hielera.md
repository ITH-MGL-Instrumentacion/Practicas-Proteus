# Modelado y control didáctico de temperatura

Esta práctica toma como base la caracterización previa de sensores de temperatura y propone un siguiente paso natural: usar un sistema térmico sencillo para estudiar respuesta dinámica y estrategias básicas de control. El objetivo no es construir una incubadora industrial, sino obtener un montaje económico y didáctico que permita relacionar sensor, actuador, planta y controlador.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Prácticas de sensores de temperatura](../8_Temperatura/8_Practicas_Temperatura.md) | [Volver al índice](../README.md) | [Control PID de la hielera](../12_Control_hielera/12_Control_hielera.md) |

<h2>Índice</h2>

- [Objetivo](#objetivo)
- [1. Sistema térmico propuesto](#1-sistema-térmico-propuesto)
	- [Montaje didáctico](#montaje-didáctico)
	- [Herramientas de software](#herramientas-de-software)
	- [Variables del sistema](#variables-del-sistema)
- [2. Captura de la respuesta al escalón](#2-captura-de-la-respuesta-al-escalón)
- [3. Modelo matemático — Sistema de segundo orden sobreamortiguado](#3-modelo-matemático--sistema-de-segundo-orden-sobreamortiguado)
- [4. Identificación experimental con el sintonizador](#4-identificación-experimental-con-el-sintonizador)
- [5. Sintonía PID por método IMC](#5-sintonía-pid-por-método-imc)
- [6. Autotune por método del relé (opcional)](#6-autotune-por-método-del-relé-opcional)
- [7. Actividades propuestas](#7-actividades-propuestas)

## Objetivo

Capturar la respuesta al escalón de una hielera de poliestireno con calefactor y ventilador, identificar su modelo dinámico como un sistema de segundo orden sobreamortiguado, y calcular los parámetros de un controlador PID usando el método IMC para usarlos en la práctica siguiente.

## 1. Sistema térmico propuesto

### Montaje didáctico

El sistema se construye con los siguientes elementos:

- Una hielera de poliestireno como recinto térmico con agua dentro.
- Una resistencia calefactora controlada por un MOSFET IRLZ44N desde el pin PWM `D3` del Arduino.
- Un ventilador en `D5` que corre al máximo durante todo el experimento para homogeneizar la temperatura del agua.
- Un sensor LM35 en `A0` como sensor de temperatura (`10 mV/°C`, referencia de `5 V`).

### Herramientas de software

| Archivo | Función |
|---|---|
| [`11_Modelado_hielera.ino`](11_Modelado_hielera.ino) | Aplica el escalón de PWM y transmite CSV por Serial |
| [`11_logger.py`](11_logger.py) | Captura el CSV desde el puerto serie y lo guarda en `datos_hielera.csv` |
| [`sintonizador.ipynb`](sintonizador.ipynb) | Ajusta el modelo y calcula los parámetros PID por IMC |

### Variables del sistema

| Variable | Símbolo | Descripción |
|---|---|---|
| Entrada | $u$ | Ciclo útil PWM (0–255) del calefactor |
| Salida | $T$ | Temperatura del agua medida por el LM35 (°C) |
| Temperatura inicial | $T_{amb}$ | Temperatura del agua al inicio del experimento |
| Perturbación | — | Temperatura ambiente, intercambio de calor con el poliestireno |

## 2. Captura de la respuesta al escalón

El programa [`11_Modelado_hielera.ino`](11_Modelado_hielera.ino) realiza el siguiente procedimiento automáticamente:

1. Espera `5 s` con el calefactor apagado para registrar la temperatura inicial.
2. Aplica un escalón de `PWM = 128` (50 %) al calefactor.
3. Envía por Serial una línea CSV cada `100 ms` con el formato `Tiempo_s,Temperatura_C,Entrada_PWM`.
4. Detecta el estado estacionario con una ventana deslizante de `150` muestras y umbral `0.15 °C`, o termina al llegar a `900 s` o `85 °C`.
5. Al terminar envía la línea `FIN,MOTIVO`.

El script [`11_logger.py`](11_logger.py) escucha el puerto serie y guarda todo en `datos_hielera.csv`. Parámetros a ajustar antes de ejecutarlo:

```python
PUERTO_COM     = 'COM3'          # Puerto del Arduino
ARCHIVO_SALIDA = 'datos_hielera.csv'
```

Ejecución:

```bash
python 11_logger.py
```

## 3. Modelo matemático — Sistema de segundo orden sobreamortiguado

La hielera se modela como dos primeros órdenes en cascada: uno representa la inercia térmica del agua y el otro la dinámica del calefactor o del sensor. En el dominio de Laplace:

$$
G(s) = \frac{K}{(\tau_1 s + 1)(\tau_2 s + 1)}
$$

donde:

| Parámetro | Descripción | Unidades |
|---|---|---|
| $K$ | Ganancia estática | °C / PWM |
| $\tau_1$ | Constante de tiempo dominante (masa de agua) | s |
| $\tau_2$ | Constante de tiempo secundaria (calefactor/sensor) | s, con $\tau_1 > \tau_2$ |

Para una entrada escalón de amplitud $U$ con condición inicial $T_{amb}$, la respuesta temporal es:

$$
y(t) = T_{amb} + K \cdot U \left(1 - \frac{\tau_1\, e^{-t/\tau_1} - \tau_2\, e^{-t/\tau_2}}{\tau_1 - \tau_2}\right)
$$

## 4. Identificación experimental con el sintonizador

El notebook [`sintonizador.ipynb`](sintonizador.ipynb) automatiza la identificación completa:

1. **Carga el CSV** y detecta automáticamente el valor del escalón (`U_STEP = max(Entrada_PWM)`).
2. **Filtra los datos** anteriores al escalón y define el tiempo relativo $t' = t - 5$.
3. **Ajusta el modelo** usando `scipy.optimize.curve_fit` sobre la ecuación de segundo orden, partiendo de estimaciones iniciales $\tau_1 = 40$ s, $\tau_2 = 15$ s.
4. **Calcula $R^2$** para cuantificar la bondad del ajuste.
5. **Calcula los parámetros PID** por el método IMC (ver sección siguiente).
6. **Muestra tablas y gráficas** con los resultados formateados.

> Si el ajuste es pobre ($R^2 < 0.99$), revisa que el CSV contenga la curva completa hasta el estado estacionario. Los primeros `5 s` (antes del escalón) se descartan automáticamente.

## 5. Sintonía PID por método IMC

El método **IMC (Internal Model Control)** parte del modelo identificado y produce parámetros analíticos sin necesidad de prueba y error. El único parámetro de diseño es $\lambda$, la constante de tiempo deseada del lazo cerrado. Una regla práctica robusta es:

$$
\lambda = \frac{\tau_1}{3}
$$

Con ese valor, los parámetros del controlador PID en forma paralela son:

$$
K_p = \frac{\tau_1 + \tau_2}{K \cdot \lambda}
\qquad
T_i = \tau_1 + \tau_2
\qquad
T_d = \frac{\tau_1 \tau_2}{\tau_1 + \tau_2}
$$

$$
K_i = \frac{K_p}{T_i}
\qquad
K_d = K_p \cdot T_d
$$

Un $\lambda$ más pequeño da respuesta más rápida pero más sensible al ruido; uno más grande da respuesta más suave. Los valores finales que calcula el notebook se transfieren directamente al código de la práctica 12.

## 6. Autotune por método del relé (opcional)

Como alternativa al sintonizador, el archivo [`11_Modelado_hielera_2.ino`](11_Modelado_hielera_2.ino) implementa el método del relé (Åström–Hägglund) para obtener los parámetros PID directamente desde el hardware sin necesidad de identificar el modelo:

1. Calienta a máxima potencia hasta entrar en la banda `SP ± 2 × h`.
2. Aplica control bang-bang con histéresis `±h` alrededor del setpoint.
3. Descarta los primeros 2 ciclos y promedia los siguientes 4.
4. Calcula $T_u$, $K_u$ y los parámetros PID por Ziegler-Nichols.

El script [`11_logger_2.py`](11_logger_2.py) captura los datos y muestra los resultados al terminar.

> El método del relé tiende a dar parámetros más agresivos que IMC. Para comparar ambos en el mismo punto de operación, usa `PWM_ON = 128` en el `.ino`.

## 7. Actividades propuestas

1. Conecta el hardware (LM35 en `A0`, MOSFET en `D3`, ventilador en `D5`) y carga [`11_Modelado_hielera.ino`](11_Modelado_hielera.ino).
2. Ejecuta `python 11_logger.py` y espera a que el experimento termine automáticamente (el Arduino enviará `FIN,ESTADO_ESTACIONARIO`).
3. Abre [`sintonizador.ipynb`](sintonizador.ipynb) y ejecuta todas las celdas. Anota los valores de $K$, $\tau_1$, $\tau_2$ y $R^2$.
4. Copia los parámetros $K_p$, $K_i$, $K_d$ que calcula el notebook y transfiérelos a la práctica 12.
5. (Opcional) Repite el experimento con `PWM_ESCALON = 200` y compara si el modelo cambia.
6. (Opcional) Ejecuta el autotune por relé (`11_Modelado_hielera_2.ino` + `11_logger_2.py`) y compara los parámetros obtenidos con los del IMC.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Prácticas de sensores de temperatura](../8_Temperatura/8_Practicas_Temperatura.md) | [Volver al índice](../README.md) | [Control PID de la hielera](../12_Control_hielera/12_Control_hielera.md) |