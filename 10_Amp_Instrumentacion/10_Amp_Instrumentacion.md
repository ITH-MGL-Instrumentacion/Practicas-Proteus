<h1>Amplificador de instrumentación</h1>

En esta sección se introduce el amplificador de instrumentación como bloque de acondicionamiento para sensores que entregan señales diferenciales pequeñas. La idea es entender por qué no basta con un amplificador cualquiera y cómo este bloque se vuelve especialmente útil cuando un sensor resistivo, como un NTC dentro de un puente de Wheatstone, genera solo una diferencia de voltaje muy pequeña.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Modelado y control de temperatura](../9_Temperatura_Modelado_Control/9_Temperatura_Modelado_Control.md) | [Volver al índice](../README.md) | [RTD y termorresistencias](../11_RTD/11_RTD.md) |

<h2>Índice</h2>

- [Objetivo](#objetivo)
- [1. ¿Qué es un amplificador de instrumentación?](#1-qué-es-un-amplificador-de-instrumentación)
- [2. ¿Por qué no basta un amplificador operacional simple?](#2-por-qué-no-basta-un-amplificador-operacional-simple)
- [3. Modelo ideal básico](#3-modelo-ideal-básico)
- [4. Ganancia y rechazo al modo común](#4-ganancia-y-rechazo-al-modo-común)
- [5. Puente de Wheatstone con NTC](#5-puente-de-wheatstone-con-ntc)
- [6. Práctica: NTC en puente y amplificador de instrumentación](#6-práctica-ntc-en-puente-y-amplificador-de-instrumentación)
- [7. Algoritmo general](#7-algoritmo-general)
- [8. Actividades](#8-actividades)

## Objetivo

Entender el funcionamiento básico de un amplificador de instrumentación y aplicarlo al acondicionamiento de un NTC conectado en un puente de Wheatstone para convertir una pequeña señal diferencial en una salida útil para un microcontrolador o instrumento de medición.

## 1. ¿Qué es un amplificador de instrumentación?

Un amplificador de instrumentación es un amplificador diferencial de alta precisión diseñado para medir diferencias pequeñas de voltaje entre dos nodos, incluso cuando ambos están montados sobre un nivel común relativamente grande.

Su uso es muy común en instrumentación porque muchos sensores no entregan una señal fuerte respecto a tierra, sino una diferencia pequeña entre dos puntos del circuito. Ese es el caso de puentes resistivos, galgas extensiométricas, RTD, termopares amplificados y algunos acondicionamientos con NTC.

Sus características más importantes son:

- Alta impedancia de entrada.
- Buena estabilidad de ganancia.
- Alto rechazo al modo común.
- Salida fácil de llevar a un ADC.

## 2. ¿Por qué no basta un amplificador operacional simple?

En principio, un amplificador operacional podría usarse para amplificar una señal pequeña. El problema es que en instrumentación no solo importa la ganancia, sino también qué tanto ruido común, offset y errores de resistencia se agregan al sistema.

Si los dos nodos del sensor tienen casi el mismo voltaje y la diferencia entre ellos es pequeña, un amplificador de instrumentación permite:

- amplificar principalmente la diferencia útil,
- rechazar mejor señales comunes a ambas entradas,
- evitar cargar excesivamente el puente o el sensor,
- y facilitar el ajuste de ganancia.

Por eso este bloque aparece con frecuencia justo después de un puente de Wheatstone.

## 3. Modelo ideal básico

La ecuación ideal más importante es:

$$
V_{out}=G\left(V^{+}-V^{-}\right)+V_{ref}
$$

Donde:

- $V^{+}$ es la entrada no inversora del bloque diferencial.
- $V^{-}$ es la entrada inversora.
- $G$ es la ganancia.
- $V_{ref}$ es una referencia opcional para desplazar la salida.

Si no se usa desplazamiento de referencia y el sistema es simétrico, muchas veces se trabaja simplemente con:

$$
V_{out}=G\left(V^{+}-V^{-}\right)
$$

Esta ecuación deja clara la idea principal: el bloque amplifica diferencia de voltaje, no el valor absoluto respecto a tierra.

## 4. Ganancia y rechazo al modo común

En un circuito real interesa que el amplificador responda mucho a la diferencia entre entradas, pero muy poco a la parte común. Si definimos:

$$
V_d=V^{+}-V^{-}
$$

y

$$
V_{cm}=\frac{V^{+}+V^{-}}{2}
$$

entonces idealmente el amplificador debería depender solo de $V_d$. En la práctica siempre existe algo de sensibilidad al modo común, por eso se habla de rechazo al modo común o `CMRR`.

Cuanto mayor sea el `CMRR`, mejor podrá el circuito ignorar ruido o desplazamientos presentes en ambas entradas al mismo tiempo.

En muchos amplificadores de instrumentación integrados, la ganancia se fija con una resistencia externa. En una aproximación didáctica basta con entender que:

- una mayor ganancia hace visible una señal diferencial pequeña,
- pero también puede amplificar ruido y offset si el sistema no está bien diseñado.

## 5. Puente de Wheatstone con NTC

Un puente de Wheatstone es una forma muy útil de convertir pequeños cambios de resistencia en una señal diferencial de voltaje.

Si se tienen dos divisores alimentados con el mismo voltaje $V_{exc}$, la salida diferencial del puente puede escribirse como:

$$
V_{bridge}=V_{exc}\left(\frac{R_2}{R_1+R_2}-\frac{R_4}{R_3+R_4}\right)
$$

Cuando el puente está balanceado, la salida es cero. Si uno de los brazos cambia, por ejemplo porque un NTC modifica su resistencia con la temperatura, entonces aparece un pequeño desequilibrio.

En esta práctica, el NTC puede colocarse en uno de los brazos del puente y una resistencia fija de valor similar al del NTC nominal puede colocarse en la rama opuesta para centrar la medición alrededor de una temperatura de interés.

Por ejemplo, si el NTC es de `10 kΩ` a `25 °C`, una primera elección razonable es balancear el puente cerca de ese valor usando resistencias del mismo orden. Así, alrededor de la temperatura ambiente la señal diferencial es pequeña pero muy sensible a los cambios térmicos.

La cadena de medición queda así:

1. El NTC cambia su resistencia con la temperatura.
2. El puente convierte ese cambio en una diferencia de voltaje.
3. El amplificador de instrumentación amplifica esa diferencia.
4. El ADC mide una señal mucho más fácil de procesar.

## 6. Práctica: NTC en puente y amplificador de instrumentación

La práctica propuesta consiste en armar un puente de Wheatstone usando un NTC de `10 kΩ` y después amplificar su salida diferencial con un amplificador de instrumentación.

Conexión sugerida:

- Un NTC de `10 kΩ` en uno de los brazos del puente.
- Resistencias fijas en los otros brazos, idealmente del mismo orden.
- Alimentación común del puente con `5 V`.
- Salida diferencial del puente conectada a las entradas del amplificador de instrumentación.
- Salida del amplificador conectada a `A0` o a la entrada analógica que se desee usar.

La idea didáctica no es solo leer temperatura, sino observar estas dos etapas:

1. El puente genera una diferencia muy pequeña.
2. El amplificador la convierte en una señal suficientemente grande para visualizarse mejor.

Se puede comparar además la salida del puente contra la salida del amplificador con el osciloscopio virtual o voltímetros en Proteus.

Imagen sugerida: `img/puente_ntc_amp_instrumentacion.png`

Imagen sugerida: `img/salida_puente_vs_salida_amplificada.png`

## 7. Algoritmo general

```text
INICIO
	Definir ganancia del amplificador
	Definir pin analogico de lectura
	Definir modelo del NTC o calibracion voltaje-temperatura

	setup:
		Iniciar comunicacion serial

	loop:
		Leer ADC de la salida amplificada
		Convertir ADC a voltaje
		Relacionar el voltaje con el desequilibrio del puente
		Convertir ese desequilibrio a resistencia o temperatura
		Mostrar resultados
FIN
```

Hay dos formas de trabajar la práctica:

1. Enfoque analítico.
   A partir de la ecuación del puente, se intenta recuperar la resistencia del NTC y luego se convierte a temperatura.
2. Enfoque de calibración.
   Se registran varios pares temperatura-voltaje y se ajusta una curva o una recta local alrededor del punto de operación.

Para una práctica introductoria, el segundo enfoque suele ser más sencillo cuando el objetivo principal es entender el acondicionamiento analógico.

## 8. Actividades

1. Explica por qué un puente de Wheatstone convierte un cambio de resistencia en una diferencia de voltaje.
2. Explica por qué la salida del puente suele ser pequeña incluso cuando el sensor cambia.
3. Describe con tus palabras qué hace el amplificador de instrumentación y qué no hace.
4. Compara la salida del puente antes y después del amplificador de instrumentación.
5. Justifica por qué conviene balancear el puente cerca de la temperatura de interés.
6. Implementa una lectura con Arduino a partir de la salida amplificada y compara entre convertir por ecuación o por calibración.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Modelado y control de temperatura](../9_Temperatura_Modelado_Control/9_Temperatura_Modelado_Control.md) | [Volver al índice](../README.md) | [RTD y termorresistencias](../11_RTD/11_RTD.md) |
