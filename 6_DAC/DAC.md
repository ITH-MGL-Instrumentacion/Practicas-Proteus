<h1>Convertidores digital-analógico</h1>

![](img/DAC.png)

En esta sección se explica el funcionamiento de algunos convertidores digital-analógico (DAC por sus siglas en inglés). De esta forma, se creará un generador de funciones por medio de configuraciones con amplificadores operacionales, siendo un paso importante el filtrado para que no se vean escalones.

<h2>Índice</h2>

- [1. DAC con PWM](#1-dac-con-pwm)
- [2. DAC por suma ponderada](#2-dac-por-suma-ponderada)


## 1. DAC con PWM
Es el más sencillo de hacer con un Arduino. El PWM de Arduino UNO es de 8 bits y tiene dos frecuencias: 490 Hz para los pines ~3, ~9, ~10 y ~11, y 980 Hz para los pines ~5 y ~6.

![PWM Arduino](img/PWM.png)

Mientras mayor la frecuencia del PWM, mayor la frecuencia de la señal que podemos crear, por lo que esta vez se usará la señal del pin 5 de 980 Hz. 

Simplemente, se selecciona el ancho de pulso deseado correspondiente a la señal entre 0 V y 5 V. Después, se pasa por un filtro pasa-bajas para que se promedie el voltaje, lo que hará que la señal en vez de verse como un montón de picos, se verá como una señal un poco más analógica. 

En la siguiente imagen puede verse la diferencia entre la señal PWM del Arduino (amarilla) y la señal filtrada (azul).

![](img/DAC_PWM_simulacion.png)

El problema de este método es que debe de haber un equilibrio entre qué tanto se suaviza la señal y la velocidad de carga del condensador, además de que la frecuencia resultante será muchísimo más baja que la ya lenta del PWM, por lo que el resultado será una frecuencia muy baja para mantener la resolución. Es por ello que se recomienda más usar los otros métodos.

En el archivo de Proteus [DAC_PWM.pdsprj](DAC_PWM.pdsprj) se muestra una simulación del PWM del Arduino como DAC, donde en el pin 5 es la salida PWM y en las entradas analógicas A0 y A1 hay potenciómetros para controlar la frecuencia ($1-40$ Hz) de la señal senoidal y la amplitud ($0-5$ V) respectivamente.

![](img/DAC_PWM_IO.png)

El filtro usa varios OpAmps. El primero es un seguidor para poder medir correctamente con el osciloscopio la señal PWM del Arduino y que los condensadores no causen interferencias. Luego podemos usar los selectores para elegir entre dos opciones:
1. La de arriba son filtros pasivos de primer orden en cascada, formando un filtro de segundo orden. Es necesario aislar entre sí cada etapa usando seguidores para facilitar los cálculos.
2. La de abajo es un filtro activo de segundo orden, un pasa-bajas de Butterworth. Los cálculos se hicieron gracias a la página de [Wilaeba Electrónica](https://wilaebaelectronica.blogspot.com/2017/01/filtro-pasa-bajos-activo-de-2do-orden-sallen-key.html) y permite usar un solo OpAmp.

Ambos dan resultados muy similares.

![](img/DAC_PWM_filtros.png)

## 2. DAC por suma ponderada
En este caso, se usa un OpAmp en modo de sumador. Básicamente, se considera cada salida digital del Arduino como un bit, por lo que si queremos un ADC de un byte (8 bits), necesitamos 8 pines de salida.

Como resultado, se verá una señal con 255 escalones ($2^8-1$), por lo que la resolución será bastante mejor incluso sin filtrar. 

![](img/DAC_suma_escalones.png)

Si queremos hacer una señal senoidal o triangular, es posible usar un filtro pasa-bajas para hacer la señal aún más limpia. 

![](img/DAC_suma_filtrada.png)

Además, como ya no dependemos de la frecuencia del PWM, se pueden usar velocidades de respuesta más altas, aunque sigue dependiendo de la velocidad del microcontrolador y de la cantidad de bits que usemos (a más bits, más tardado); también al ser voltajes ya bastante cercanos a los deseados (a diferencia del PWM, que solo varía entre 0 V y 5 V), el filtro se vuelve más estable.

Ahora, el circuito es sencillo y fácil de calcular, pero el problema del DAC por suma ponderada es que las resistencias son difíciles de conseguir.

