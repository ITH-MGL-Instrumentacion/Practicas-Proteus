# Conversión de señales analógicas y digitales con Arduino y Proteus

En repositorio se muestran varias prácticas enfocadas en aprender a usar de forma básica la programación de la placa de desarrollo **Arduino UNO** y el simulador de circuitos electrónicos **Proteus** con el objetivo de aprender el funcionamiento de los convertidores **analógico-digital** o **digital-analógico**.

## Entrega de tareas

Para entregar las tareas, es necesario tener un **commit** con los **archivos de arduino y proteus modificados** y un **video** con la **simulación de proteus** que muestre la versión modificada y completa (el video debe estar explicado y ser corto).

## Temas de Introducción

1. [Introducción a Arduino](0_Introduccion/Introduccion_Arduino.md)
2. [Introducción a Proteus](0_Introduccion\Introduccion_Proteus.md)
3. [Formato de los proyectos en Visual Studio Code](0_Introduccion\Formato_proyectos.md)

## Prácticas básicas

1. [Salida digital](1_Salida_Digital\Salida_Digital.md): Control del tiempo de encendido y apagado de un LED.
2. [Entrada digital](2_Entrada_Digital\Entrada_Digital.md): Control de LED usando un botón.
3. [Salida "analógica"](3_Salida_PWM\Salida_PWM.md): Control de iluminación de LED usando PWM.
4. [Entrada analógica](4_Entrada_Analogica\Entrada_Analogica.md): Lectura analógica de potenciómetro.

## Prácticas usando amplificadores operacionales

Estas prácticas están enfocadas en entender cómo las señales pasan de ser analógicas a digitales o viceversa, de forma que el microcontrolador pueda leerlas.

### Básicos

5. [Amplificadores operacionales básicos](5_OpAmps_Basicos\OpAmps_Basicos.md): Utiliza un Amplificador operacional para cada modo básico.

### Convertidor digital - analógico

6. [CDA por suma ponderada](6_CDA_suma_ponderada\CDA_suma_ponderada.md): Convertidor digital analógico usando el método poco óptimo pero intuitivo de la suma ponderada.
7. [CDA R-2R](7_CDA_R_2R\CDA_R_2R.md): Convertidor digital analógico usando el método optimizado para el código binario.

### Convertidor analógico - digital

8. [CAD flash](8_CAD_flash\CAD_flash.md): Convertidor analógico digital usando comparadores en paralelo.
9. [CAD SAR](9_CAD_SAR\CAD_SAR.md): Convertidor analógico digital usando el método de aproximaciones sucesivas buscando si es mayor o menor.
10. [CAD rampa](10_CAD_rampa\CAD_rampa.md): Convertidor analógico digital usando una rampa de voltaje y un comparador para calcular el tiempo.