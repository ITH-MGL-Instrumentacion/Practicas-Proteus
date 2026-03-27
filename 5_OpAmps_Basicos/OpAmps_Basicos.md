<h1>Amplificadores operacionales básicos</h1>

Esta sección es más técnica que las anteriores ya que tiene como objetivo entender las configuraciones básicas de un amplificador operacional. En este caso, no se programará nada en Arduino, pero sí se realizarán cambios en las resistencias para entender las ecuaciones matemáticas.

> Subirán los **videos** de cada archivo de Proteus explicando las cosas con los valores de resistencias cambiados sin modificar los archivos de proteus (Salir sin guardar)

<h2>Índice</h2>

- [Archivos](#archivos)
- [Tipos de circuitos integrados](#tipos-de-circuitos-integrados)
  - [1. LM741 (Obsoleto)](#1-lm741-obsoleto)
  - [2. LM358 y LM324 (Uso general)](#2-lm358-y-lm324-uso-general)
  - [3. LM339 y LM393 (Especialistas en comparar)](#3-lm339-y-lm393-especialistas-en-comparar)
  - [4. TL081 y TL082](#4-tl081-y-tl082)
- [Aplicaciones clásicas](#aplicaciones-clásicas)
  - [1) Básicos](#1-básicos)
    - [Comparador](#comparador)
    - [Prueba](#prueba)
    - [Seguidor de voltaje](#seguidor-de-voltaje)
    - [Prueba sugerida](#prueba-sugerida)
  - [2) Amplificadores](#2-amplificadores)
    - [Amplificador inversor](#amplificador-inversor)
    - [Ejercicio](#ejercicio)
    - [Amplificador No inversor](#amplificador-no-inversor)
    - [Ejercicio](#ejercicio-1)
  - [3) Amplificador sumador/restador](#3-amplificador-sumadorrestador)
    - [Amplificador sumador inversor](#amplificador-sumador-inversor)
    - [Amplificador sumador no inversor](#amplificador-sumador-no-inversor)
    - [Amplificador sumador restador](#amplificador-sumador-restador)
    - [Resistencia de Balance (Rx)](#resistencia-de-balance-rx)
    - [Ejercicio](#ejercicio-2)
  - [4) Filtros](#4-filtros)
    - [Integrador](#integrador)
    - [Derivador](#derivador)
- [Qué deben entregar](#qué-deben-entregar)


## Archivos

Se dividirán los OpAmps en 4 archivos según su tipo_
- [Comparador y seguidor de voltaje](OpAmps_basicos_1_Comparador_y_seguidor.pdsprj)
- [Amplificadores](OpAmps_basicos_2_Amplificadores.pdsprj)
- [Sumadores](OpAmps_basicos_3_Sumador.pdsprj)
- [Integrador y derivador](OpAmps_basicos_4_integrador_y_derivador.pdsprj)

## Tipos de circuitos integrados

Existen distintos modelos que se suelen utilizar para las prácticas, entre los que se encuentran los siguientes:

### 1. LM741 (Obsoleto)

El LM741 es el más famoso, pero para instrumentación moderna es pésimo.Problema: No es "Rail-to-Rail". Si lo alimentas con 5V, su salida máxima será de unos 3.5V y la mínima de 1.5V. Además, requiere fuente simétrica ($+15V$ y $-15V$) para funcionar bien.

> Uso: Solo para enseñar historia de la electrónica o circuitos con voltajes muy altos y no se usará en el curso.

### 2. LM358 y LM324 (Uso general)

El LM358 (2 OpAmps en el encapsulado) y el LM324 (4 OpAmps en el encapsulado) son los más usados en prácticas generales como Arduino.

Ventaja "Single Supply": Pueden funcionar solo con GND y $5V$, lo que permite llegar casi a $0V$ (GND), lo cual es vital para leer sensores que empiezan en cero. 

Su salida en voltaje positivo disminuye un poco (con $12V$ de entrada puede dar $10.5V$ de salida), pero en voltaje negativo es casi la misma en la entrada que en la salida máxima;

> Son ideales para el DAC R-2R y filtros.

### 3. LM339 y LM393 (Especialistas en comparar)

> Estos NO son OpAmps, son COMPARADORES.

Diferencia crucial: Un OpAmp intenta mantener un equilibrio. Un comparador es "violento": su salida es Colector Abierto. 

¿Qué significa? Que la salida no da voltaje por sí sola; es como un interruptor que conecta a tierra. 

Necesitas una resistencia de pull-up a 5V para que el Arduino pueda leer un "1".

Uso: Son muchísimo más rápidos que un OpAmp para el ADC Flash y el ADC SAR. Si usas un LM358 como comparador, será lento y la señal se verá "redondeada".

### 4. TL081 y TL082
Son OpAmps con entrada JFET, por lo que no consumen nada de corriente del colector, lo que los hace ideales para sensores extremadamente sensibles.

> El TL081 contiene un solo OpAmp, mientras que el TL082 tiene dos.


## Aplicaciones clásicas


### 1) Básicos

#### Comparador

![Comparador](img/OpAmp_comparador.png)

Primero resta el valor de la entrada no inversora (+) con la entrada inversora (-) y si el resultado es positivo, la salida es el voltaje máximo positivo, mientras que si el resultado es negativo, la salida es el voltaje máximo negativo

Esto la forma más sencilla de convertir una entrada analógica a una digital.

Este ejercicio se resuelve completando [OpAmps_basicos_1_Comparador_y_seguidor.pdsprj](OpAmps_basicos_1_Comparador_y_seguidor.pdsprj).

#### Prueba

- Cambia el valor del sensor de temperatura y del potenciómetro y verifica su funcionamiento.

#### Seguidor de voltaje

![OpAmp Seguidor](img/OpAmp_Seguidor.png)

El voltaje salida sigue a la entrada casi con el mismo valor.

También sirve como **buffer**: aisla etapas (alta impedancia de entrada y baja de salida). Esto es muy útil para sensores basados en resistencia o cuando un cambio en las resistencias usadas para la configuración de los OpAmps pueden alterar los valores del sensor.

Si existe una resistencia de entrada $R_s$, puede usarse una resistencia en la retroalimentación para eliminar el sesgo de corriente y balancear el circuito en aplicaciones más precisas.

Esta práctica se encuentra en [OpAmps_basicos_1_Comparador_y_seguidor.pdsprj](OpAmps_basicos_1_Comparador_y_seguidor.pdsprj).

#### Prueba sugerida
- Mueve la entrada DC y verifica que la salida cambie igual.


### 2) Amplificadores

#### Amplificador inversor

Como su nombre lo indica, amplifica el voltaje en relación a la configuración de las resistencias. Utiliza la entrada inversora ($-$)

También la salida se invierte respecto a la entrada (180° de desfase en AC). Eso significa que si la entrada es un voltaje positivo, la salida es negativo y viceversa.

![Amplificador_inversor](img/Amplificador_inversor.png)

La ganancia usa un factor:

$$G = -\frac{R_F}{R_1}$$

También se suele poner una resistencia RX que sirve para balancear el circuito eliminando el sesgo de corriente, útil en aplicaciones más precisas. Su ecuación es:

$$RX = R_F||R_1 = \frac{1}{\frac{1}{R_F} + \frac{1}{R_1}}$$

Recordemos que el símbolo $R_A||R_B$ se suele utilizar para dos resistencias en paraleleo.

El archivo se encuentra en [OpAmps_basicos_2_Amplificadores.pdsprj](OpAmps_basicos_2_Amplificadores.pdsprj).

#### Ejercicio

- Cambia los valores del potenciómetro para ver cómo cambia.
- Haz que se amplifique en un factor de 5 veces.
- Responde a las preguntas que aparecen en el circuito.

#### Amplificador No inversor

Es el equivalente del anterior, pero la salida mantiene la fase de la entrada.

La amplificación se ligeramente un poco más complicada al usar la entrada no inversora.

![Amplificador no inversor](img/Amplificador_no_inversor.png)

La ganancia usa un factor:

$$G = 1+\frac{R_F}{R_1}$$

También se suele poner una resistencia RX para balancear el circuito, cuya ecuación es:

$$RX = R_F||R_1$$

El archivo se encuentra en [OpAmps_basicos_2_Amplificadores.pdsprj](OpAmps_basicos_2_Amplificadores.pdsprj).

#### Ejercicio

- Cambia los valores del potenciómetro para ver cómo cambia.
- Haz que se amplifique en un factor de 5 veces.
- Responde a las preguntas que aparecen en el circuito.

### 3) Amplificador sumador/restador

El Amplificador sumador/restador, como su nombre indica, primero amplifica cada entrada y luego las suma o resta dependiendo de si se encuentran en la entrada no inversora ($+$) o en la inversora ($-$). 

Se recomienda que cada entrada pase por un seguidor o buffer, ya que al tener una malla compleja de resistencias, los cálculos se vuelven más complicados e inestables, por lo que es más fácil estabilizar los voltajes con el seguidor.

El caso general es el Amplificador sumador-restador que se muestra en la siguiente imagen, pero se irán viendo casos más especificos y útiles. 

![alt text](img/OpAmp_Sumador_restador.svg)

Para un circuito sumador/restador con múltiples entradas ponderadas:

$$V_{out} = \sum_{i=1}^n G_i V_i = G_1 V_1 + G_2 V_2 + \dots + G_n V_n$$

donde:
- $R_i = \frac{R_F}{G_i}$ es la resistencia de entrada para cada ganancia
- $V_{out}$ es el voltaje de salida
- $R_F$ es la resistencia de retroalimentación. Idealmente, $R_F$ debe ser un múltiplo de $\sum G_i$ o la sumatoria de todas las ganancias (positivas en suma y negativas en resta).

El archivo de proteus donde se trabajará es [OpAmps_basicos_3_Sumador.pdsprj](OpAmps_basicos_3_Sumador.pdsprj).

#### Amplificador sumador inversor

![Amplificador Sumador](img/Amplificador_sumador_inversor.png)

En su modo inversor (desfasado 180°), se usa la entrada inversora y la ecuación es muy parecida a la del [amplificador inversor](#amplificador-no-inversor) normal, solo que la amplificación se puede aplicar en cada entrada. 

Como se explicó anteriormente, $R_i = \frac{R_F}{G_i}$, por lo que se puede despejar para calcular el valor deseado.

La resistencia $R_X$ se usa para balancear la corriente del circuito, por lo que se recomienda ponerla con el valor de las demás resistencias en paralelo, pero no va a alterar mucho el voltaje resultante.

$$R_X = R_1||R_2||\cdots||R_n||R_F$$

#### Amplificador sumador no inversor

![Amplificador_sumador_no_inversor](img/Amplificador_sumador_no_inversor.png)

En este caso, se usa la entrada no inversora; no habrá desfase y los cálculos de $R_F$, $R_i$ y $G_i$ serán iguales, pero la resistencia $R_X$, además de balancear la corriente, también balancea el voltaje, por lo que debe usarse una resistencia más precisa. 

$$R_X = \frac{R_F R_P}{R_F - R_P}$$

donde $R_P$ es el paralelo de las resistencias de entrada

$$R_P = R_1||R_2||\cdots||R_n$$


#### Amplificador sumador restador

Es la versión completa de los casos anteriores.

![alt text](img/OpAmp_Sumador_restador.svg)

Para un circuito sumador/restador con múltiples entradas ponderadas:

$$V_{out} = \sum_{i=1}^n G_i V_i = G_1 V_1 + G_2 V_2 + \dots + G_n V_n$$

donde:
- $R_i = \frac{R_F}{G_i}$ es la resistencia de entrada para cada ganancia
- $V_{out}$ es el voltaje de salida
- $R_F$ es la resistencia de retroalimentación. Idealmente, $R_F$ debe ser un múltiplo de $\sum G_i$.

#### Resistencia de Balance (Rx)

El cálculo de $R_x$ depende de la suma ponderada de ganancias:

$$\sum G_i = \sum_{+} G_i - \sum_{-} G_i$$

Existen tres casos especiales:

$$R_x = \begin{cases}
  R_F & \text{si } \sum G_i = 0 \text{ → Se conecta a no inversora}\\
  0  & \text{si } \sum G_i = -1 \text{ → No inversora a tierra sin resistencia}\\
  \frac{R_F}{1 + \sum G_i} & \text{caso general → Se conecta en el signo contrario}
\end{cases}$$

Puedden basarse en la imagen, donde se muestra el signo de la resistencia $R_x$.

#### Ejercicio

- Conecta 2 entradas positivas y 4 negativas con la ganancia $G=10$ de cada entrada ya asignada y calcula la resistencia RX. Verifica que el resultado es correcto.
- Ahora conecta 2 entradas negativas y 4 positivas con una ganancia $G=10$ en cada entrada. De ser posible, intenta que las resistencias no usen decimales infinitos usando múltiplos. 


### 4) Filtros

#### Integrador

**Qué conectar**
- Configuración inversora con capacitor en realimentación.

**Qué observar**
- Con onda cuadrada de entrada, la salida tiende a forma triangular.
- La frecuencia afecta qué tan rápido cambia la salida.

**Prueba sugerida**
- Mantén amplitud y cambia frecuencia para ver la diferencia.

#### Derivador

**Qué conectar**
- Configuración inversora con capacitor en la entrada.

**Qué observar**
- Resalta cambios rápidos de la señal.
- Con onda triangular, la salida se parece más a una cuadrada.

**Prueba sugerida**
- Compara su comportamiento con el integrador para la misma entrada.

## Qué deben entregar

- El archivo de Proteus con las 9 configuraciones funcionales.
- Evidencia de simulación (capturas o video corto) mostrando al menos una observación por configuración.
- Breve conclusión: qué diferencia práctica notaron entre seguidor, comparador, amplificadores lineales y bloques dinámicos (integrador/derivador).

| Anterior | Índice | Siguiente |
|---|---|---|
| [Entrada analógica](../4_Entrada_Analogica/Entrada_Analogica.md) | [Volver al índice](../README.md#prácticas-usando-amplificadores-operacionales) | [CDA por suma ponderada](../6_CDA_suma_ponderada/CDA_suma_ponderada.md) |
