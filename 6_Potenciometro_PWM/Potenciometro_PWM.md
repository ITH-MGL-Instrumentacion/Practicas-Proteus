# Práctica adicional 2: potenciómetro controlando PWM en D3

## Objetivo
Usar el mismo potenciómetro conectado a A0 para:
1. Controlar el brillo de un LED en el pin PWM D3.
2. Encender una barra de 10 LEDs (pines 4 a 13) proporcionalmente al giro del potenciómetro.

## Algoritmo
```
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

### Funciones usadas
- `analogRead(A0)` – lectura analógica del potenciómetro.
- `map()` – conversión de 0–1023 a 0–255 o a 0–10.
- `analogWrite(3, valorPWM)` – aplica el ciclo de trabajo al LED PWM.
- `digitalWrite()` – controla los LEDs individuales.
- `delay(ms)` – suaviza lecturas rápidas y evita parpadeo visual brusco.

## Observaciones
- El valor de `map()` para los LEDs redondea hacia abajo; con la lectura máxima se encienden los 10 LEDs.
- El retardo de 50 ms es suficiente para una percepción estable sin afectar la respuesta.
