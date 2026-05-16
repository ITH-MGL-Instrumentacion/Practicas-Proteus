# Modelado y control didáctico de temperatura

Esta práctica toma como base la caracterización previa de sensores de temperatura y propone un siguiente paso natural: usar un sistema térmico sencillo para estudiar respuesta dinámica y estrategias básicas de control. El objetivo no es construir una incubadora industrial, sino obtener un montaje económico y didáctico que permita relacionar sensor, actuador, planta y controlador.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Prácticas de sensores de temperatura](../8_Temperatura/8_Practicas_Temperatura.md) | [Volver al índice](../README.md) | - |

<h2>Índice</h2>

- [Objetivo](#objetivo)
- [1. Sistema térmico propuesto](#1-sistema-térmico-propuesto)
	- [Montaje didáctico](#montaje-didáctico)
	- [Variables del sistema](#variables-del-sistema)
- [2. Sensores y actuadores](#2-sensores-y-actuadores)
	- [Uso principal del NTC](#uso-principal-del-ntc)
	- [Uso opcional del termopar](#uso-opcional-del-termopar)
	- [Bombilla de 12 V como actuador térmico](#bombilla-de-12-v-como-actuador-térmico)
- [3. Modelado dinámico de la incubadora](#3-modelado-dinámico-de-la-incubadora)
	- [Modelo térmico de primer orden](#modelo-térmico-de-primer-orden)
	- [Escalón de calentamiento](#escalón-de-calentamiento)
	- [Escalón inverso o enfriamiento](#escalón-inverso-o-enfriamiento)
	- [Cuándo considerar segundo orden](#cuándo-considerar-segundo-orden)
- [4. Identificación experimental](#4-identificación-experimental)
	- [Ganancia estática](#ganancia-estática)
	- [Constante de tiempo](#constante-de-tiempo)
	- [Efecto de la posición del sensor](#efecto-de-la-posición-del-sensor)
- [5. Tipos de control de temperatura](#5-tipos-de-control-de-temperatura)
	- [Control todo o nada](#control-todo-o-nada)
	- [Control con histéresis](#control-con-histéresis)
	- [Control proporcional por PWM](#control-proporcional-por-pwm)
	- [Control PI o PID introductorio](#control-pi-o-pid-introductorio)
- [6. Implementación sugerida con Arduino](#6-implementación-sugerida-con-arduino)
- [7. Actividades propuestas](#7-actividades-propuestas)

## Objetivo

Modelar de forma didáctica un sistema térmico sencillo, por ejemplo una incubadora casera hecha con una hielera de poliestireno y una bombilla incandescente de `12 V`, para identificar su dinámica aproximada y comparar estrategias básicas de control de temperatura con Arduino.

## 1. Sistema térmico propuesto

### Montaje didáctico

El sistema puede construirse con materiales económicos:

- Una hielera de poliestireno como recinto térmico.
- Una bombilla incandescente de `12 V` como fuente de calor.
- Un transistor o MOSFET para conmutar la bombilla desde Arduino.
- Un NTC como sensor principal de temperatura.
- De forma opcional, un termopar como sensor secundario de comparación.

Este montaje permite estudiar conceptos fundamentales de control sin necesidad de llegar a las temperaturas elevadas ni al hardware más costoso que exigiría una estación de soldadura.

![Imagen sugerida: montaje físico o esquema de la hielera de poliestireno con bombilla de 12 V y sensor interno](img/incubadora_montaje_didactico.png)

### Variables del sistema

En un modelo simple, las variables más importantes son:

- Entrada: potencia térmica aplicada por la bombilla.
- Salida: temperatura medida dentro del recinto.
- Perturbaciones: temperatura ambiente, apertura de la tapa, corrientes de aire y posición del sensor.

## 2. Sensores y actuadores

### Uso principal del NTC

Para esta práctica el NTC es el sensor más conveniente porque es económico, sensible en rangos moderados y suficiente para seguir la dinámica de una incubadora didáctica. Si la medición se concentra en un campo cercano a temperatura ambiente, su precisión puede ser adecuada usando el modelo beta; si se quiere cubrir un rango más amplio o reducir error, puede recurrirse a Steinhart-Hart.

### Uso opcional del termopar

Aunque un termopar suele reservarse para temperaturas más altas, aquí puede usarse como comparación didáctica. En la práctica anterior ya se introdujo junto con el NTC de unión fría y su acondicionamiento analógico. En este nuevo montaje puede aparecer como sensor alternativo o como referencia de contraste, aun cuando no sea la opción más económica ni la más simple para una incubadora.

### Bombilla de 12 V como actuador térmico

La bombilla de `12 V` funciona como actuador térmico porque transforma potencia eléctrica en calor. Si se conmuta con PWM o con ventanas de tiempo, la potencia promedio entregada al recinto puede controlarse de forma sencilla. Desde el punto de vista del modelado, esa potencia actúa como la entrada del sistema.

![Imagen sugerida: diagrama de bloques con Arduino, MOSFET, bombilla y sensor NTC](img/incubadora_bloques_control.png)

## 3. Modelado dinámico de la incubadora

### Modelo térmico de primer orden

Una primera aproximación muy útil consiste en tratar la incubadora como un sistema térmico de primer orden:

$$
C_{th}\frac{dT}{dt}=P_{in}-\frac{T-T_{amb}}{R_{th}}
$$

donde:

- $C_{th}$ es la capacidad térmica equivalente del sistema.
- $R_{th}$ es la resistencia térmica hacia el ambiente.
- $P_{in}$ es la potencia promedio entregada por la bombilla.
- $T$ es la temperatura interna.
- $T_{amb}$ es la temperatura ambiente.

Reordenando, aparece la forma clásica:

$$
G(s)=\frac{K}{\tau s + 1}
$$

con una constante de tiempo aproximada:

$$
\tau = R_{th}C_{th}
$$

Este modelo suele ser suficiente para una primera práctica porque la caja y el aire interno suelen variar mucho más lento que la electrónica de control.

![Imagen sugerida: respuesta térmica al escalón de una incubadora didáctica de primer orden](img/incubadora_respuesta_escalon.png)

### Escalón de calentamiento

Si se aplica un PWM fijo a la bombilla, puede considerarse una entrada tipo escalón de potencia promedio. La respuesta ideal del recinto se aproxima por:

$$
T(t)=T_{amb}+\Delta T_{\infty}\left(1-e^{-t/\tau}\right)
$$

donde $\Delta T_{\infty}$ es el incremento final de temperatura para ese nivel de potencia.

### Escalón inverso o enfriamiento

Si la bombilla se apaga de manera brusca, el enfriamiento puede describirse como un escalón inverso:

$$
T(t)=T_{amb}+\left(T_0-T_{amb}\right)e^{-t/\tau}
$$

Esta prueba también es muy útil porque permite verificar si el mismo valor de $\tau$ describe bien el sistema tanto en calentamiento como en enfriamiento.

### Cuándo considerar segundo orden

Si se observa una respuesta claramente más compleja, por ejemplo por retardo apreciable, por una gran masa térmica interna o por una diferencia marcada entre la dinámica del sensor y la del recinto, puede considerarse un modelo de segundo orden o un primer orden con tiempo muerto. Sin embargo, para una actividad introductoria suele bastar un modelo de primer orden bien identificado.

## 4. Identificación experimental

### Ganancia estática

La ganancia estática puede estimarse comparando el cambio final de temperatura con el cambio de entrada aplicado:

$$
K=\frac{\Delta T_{\infty}}{\Delta u}
$$

donde $\Delta u$ puede interpretarse como potencia promedio, ciclo útil o una variable proporcional al actuador.

### Constante de tiempo

La constante de tiempo se estima localizando el instante en que la respuesta alcanza aproximadamente el `63.2 %` del cambio total. Si el modelo es de primer orden, ese tiempo corresponde a $\tau$.

### Efecto de la posición del sensor

La dinámica medida no depende solo del sensor. También depende del lugar donde se coloque:

- Cerca de la bombilla: la respuesta será más rápida y la temperatura parecerá mayor.
- En el centro del recinto: la medida representará mejor la temperatura promedio del aire.
- Cerca de la pared: la lectura estará más influida por pérdidas al ambiente.

Por eso, aunque beta pertenece sobre todo al sensor, la constante de tiempo aparente y la disipación efectiva dependen del encapsulado, del medio y de la posición del sensor dentro del sistema.

## 5. Tipos de control de temperatura

### Control todo o nada

Es el control más simple. Si la temperatura medida está por debajo de una referencia, la bombilla se enciende; si está por encima, se apaga.

Ventajas:

- Muy fácil de implementar.
- Adecuado para una primera demostración.

Limitaciones:

- Produce oscilaciones alrededor de la referencia.
- Puede conmutar demasiado rápido si no se añade histéresis.

### Control con histéresis

Mejora el control todo o nada definiendo una banda alrededor de la referencia. Por ejemplo, se enciende la bombilla cuando la temperatura baja de `36.5 ^\circ C` y se apaga al superar `37.5 ^\circ C`.

Esta estrategia es especialmente útil en una práctica didáctica porque estabiliza el sistema sin complicar demasiado la programación.

### Control proporcional por PWM

En vez de encender o apagar totalmente la bombilla, puede variarse la potencia promedio usando PWM o ventanas de tiempo. Una forma simple es definir:

$$
u(t)=K_p\left(T_{ref}-T(t)\right)
$$

limitando la salida al rango permitido por el actuador.

Este método suele producir un comportamiento más suave, aunque puede dejar error en estado estacionario si no se añade acción integral.

### Control PI o PID introductorio

Como extensión didáctica, puede introducirse un controlador PI o PID. No hace falta sintonizarlo de manera industrial; basta con mostrar que:

- La acción proporcional acelera la respuesta.
- La acción integral ayuda a eliminar error permanente.
- La acción derivativa puede mejorar el amortiguamiento, aunque en sistemas térmicos sencillos a menudo no es indispensable.

Para este curso, un control PI discreto sencillo suele ser más que suficiente si se quiere ir más allá del todo o nada.

## 6. Implementación sugerida con Arduino

Una arquitectura práctica sería:

1. Medir la temperatura con el NTC.
2. Filtrar o promediar la lectura.
3. Comparar contra una referencia deseada.
4. Calcular la acción de control.
5. Excitar la bombilla mediante PWM o conmutación temporizada.

En una versión mínima, el control puede hacerse con histéresis. En una versión más avanzada, puede implementarse control proporcional o PI usando un MOSFET para manejar la carga de `12 V`.

![Imagen sugerida: esquema de potencia con MOSFET, bombilla de 12 V y diagrama de control desde Arduino](img/incubadora_potencia_mosfet.png)

## 7. Actividades propuestas

1. Construye un montaje didáctico con una hielera de poliestireno, una bombilla de `12 V` y un sensor NTC.
2. Aplica un escalón de potencia y registra la temperatura interna contra tiempo.
3. Estima la ganancia estática y la constante de tiempo del sistema.
4. Repite la prueba de calentamiento cambiando la posición del sensor y compara la respuesta.
5. Realiza una prueba de enfriamiento apagando la bombilla y verifica si el modelo de primer orden sigue siendo adecuado.
6. Implementa un control todo o nada y registra la oscilación térmica obtenida.
7. Añade histéresis y compara la mejora frente al caso anterior.
8. Si el hardware lo permite, implementa control proporcional mediante PWM y compara tiempo de establecimiento, sobreimpulso y error final.
9. Discute en qué sentido el termopar puede seguir siendo útil como elemento didáctico, aunque no sea el sensor más simple para una incubadora.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Prácticas de sensores de temperatura](../8_Temperatura/8_Practicas_Temperatura.md) | [Volver al índice](../README.md) | - |