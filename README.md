# Conversión de señales analógicas y digitales con Arduino y Proteus

En repositorio se muestran varias prácticas enfocadas en aprender a usar de forma básica la programación de la placa de desarrollo **Arduino UNO** y el simulador de circuitos electrónicos **Proteus** con el objetivo de aprender el funcionamiento de los convertidores **analógico-digital** o **digital-analógico**, así como el funcionamiento de algunos sensores.

## Entrega de tareas

Para entregar las tareas, es necesario tener un **commit** con los **archivos de arduino y proteus modificados** y un **video** con la **simulación de proteus** que muestre la versión modificada y completa (el video debe estar explicado y ser corto).

## Temas de Introducción

1. [Introducción a Arduino](0_Introduccion/Introduccion_Arduino.md)
2. [Introducción a Proteus](0_Introduccion/Introduccion_Proteus.md)
3. [Formato de los proyectos en Visual Studio Code](0_Introduccion/Formato_proyectos.md)

## Prácticas básicas

1. [Salida digital](1_Salida_Digital/Salida_Digital.md): Control del tiempo de encendido y apagado de un LED.
2. [Entrada digital](2_Entrada_Digital/Entrada_Digital.md): Control de LED usando un botón.
3. [Salida "analógica"](3_Salida_PWM/Salida_PWM.md): Control de iluminación de LED usando PWM.
4. [Entrada analógica](4_Entrada_Analogica/Entrada_Analogica.md): Lectura analógica de potenciómetro.

## Prácticas usando amplificadores operacionales

Estas prácticas están enfocadas en entender cómo las señales pasan de ser analógicas a digitales o viceversa, de forma que el microcontrolador pueda leerlas.

### Básicos

5. [Amplificadores operacionales básicos](5_OpAmps_Basicos/OpAmps_Basicos.md): Utiliza un Amplificador operacional para cada modo básico.

### Convertidor digital - analógico

6. [DAC](6_DAC/DAC.md): Se explican 3 tipos de Convertidores digital-analógico y la práctica en protoboard es el DAC R-2R.

### Convertidor analógico - digital

7. [ADC](7_ADC/ADC.md): Convertidor analógico-digital por seguimiento con el DAC R-2R.


## Prácticas de instrumentación

8. [Introducción a sensores y acondicionamiento básico](8_Sensores/8_Sensores.md): Divisor de voltaje, puente de Wheatstone y práctica introductoria con potenciómetro.
9. [Termistores](9_Termistores/9_Termistores.md): Sensores NTC y PTC, modelos básicos y práctica sencilla con NTC de 10 kΩ.
10. [Sensores semiconductores](10_Semiconductores/10_Semiconductores.md): LM35, TMP36 y comparación con sensores integrados digitales.
11. [RTD](11_RTD/11_RTD.md): Pt100, Pt1000, conexiones de 2, 3 y 4 hilos y medición resistiva de mayor precisión.
12. [Termopares](12_Termopares/12_Termopares.md): Efecto Seebeck, tipos principales y compensación de unión fría.
13. [Sensores piezorresistivos](13_Piezoresistivo/13_Piezoresistivo.md): Sensores resistivos de presión o esfuerzo basados en puente.
14. [Galga extensiométrica](14_Galga_extensiometrica/14_Galga_extensiometrica.md): Deformación mecánica, medio puente y puente completo.
15. [Incubadora](15_Incubadora/15_Incubadora.md): Aplicación didáctica de sensor, actuador y control básico de temperatura.

## Material anterior de referencia

- [Teoría de sensores de temperatura](8_Temperatura/8_Teoría_Temperatura.md)
- [Prácticas de sensores de temperatura](8_Temperatura/8_Practicas_Temperatura.md)
- [Modelado y control didáctico de temperatura](9_Temperatura_Modelado_Control/9_Temperatura_Modelado_Control.md)