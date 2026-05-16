# Guía de Calibración: Módulo de Acondicionamiento PT1000 con AD620

Este documento describe el procedimiento para calibrar el sistema de medición de temperatura de la incubadora. El objetivo es mapear el rango de **20 °C a 60 °C** a un voltaje de salida de **0V a 1V**.

## 1. Arquitectura del Módulo

El sistema cuenta con dos controles críticos:

* **Trimmer de Ganancia ($R_G$):** Ajusta el "Span" o la pendiente de la señal (qué tanto cambia el voltaje por cada grado).
* **Trimmer de Referencia ($V_{REF}$):** Ajusta el "Zero" o el pedestal de la señal (dónde empieza a contar el voltaje).

## 2. Requisitos Previos

Antes de iniciar, asegúrate de tener:

1. Multímetro digital de precisión.
2. Resistencias de calibración (o potenciómetros ajustados):
* **$1077.9 \Omega$** (Simula 20 °C).
* **$1232.4 \Omega$** (Simula 60 °C).


3. Fuente de alimentación dual encendida y estable.

---

## 3. Procedimiento de Calibración (Paso a Paso)

### Paso 1: Verificación del "Zero" Eléctrico

Este paso asegura que el AD620 no tenga errores internos de offset significativos.

1. Coloca un puente (jumper) entre las entradas **+IN** y **-IN** del módulo (0V diferencial).
2. Mide el voltaje en la **Salida (Vout)**.
3. Gira el **Trimmer de $V_{REF}$** hasta que el multímetro marque lo más cercano a **0.000 V**.

### Paso 2: Calibración del Límite Inferior (20 °C)

Ajustaremos el punto donde el Arduino empezará a medir.

1. Retira el puente de las entradas y conecta la resistencia de **$1077.9 \Omega$** (20 °C).
2. Verifica que la fuente de corriente entregue los $100 \mu A$ (midiendo ~107.8 mV en la resistencia).
3. Ajusta el **Trimmer de $V_{REF}$** hasta que la salida del módulo sea exactamente **0.000 V**.
* *Nota: Este es tu nuevo "cero" lógico.*



### Paso 3: Calibración de la Escala o "Span" (60 °C)

Ahora ajustaremos la sensibilidad para que el rango máximo sea de 1V.

1. Cambia la resistencia por la de **$1232.4 \Omega$** (60 °C).
2. Sin tocar el trimmer de referencia, gira el **Trimmer de $R_G$** hasta que la salida sea exactamente **1.000 V**.

### Paso 4: Ajuste Fino (Iteración)

Debido a que los ajustes analógicos pueden influirse levemente entre sí:

1. Regresa a la resistencia de **20 °C**. Si la salida se movió de 0V, corrige ligeramente con el trimmer de **$V_{REF}$**.
2. Regresa a la de **60 °C**. Si no marca 1V, corrige con **$R_G$**.
3. Repite hasta que ambos puntos sean precisos.

---

## 4. Configuración en Arduino

Para aprovechar esta calibración de 1V, recuerda configurar la referencia interna en tu código de Python/C++:

```cpp
void setup() {
  // Configura el ADC para usar la referencia interna de 1.1V
  // Esto da una resolución aproximada de 1.07mV por unidad.
  analogReference(INTERNAL); 
  Serial.begin(9600);
}

```

---

## 5. Resumen de Errores Comunes

* **Saturación:** Si el voltaje se queda "pegado" cerca de los 11V o -11V, el $V_{REF}$ está muy lejos de su punto de equilibrio.
* **Inestabilidad:** Si la lectura oscila, revisa que las resistencias de $1 M\Omega$ a tierra en las entradas estén bien conectadas.
* **Calentamiento:** Si el Zener o el AD620 queman al tacto, revisa la polaridad de la fuente.

---

### Tarea para el reporte:

1. Registra el valor óhmico final de $R_G$ tras la calibración.
2. Calcula la ganancia real obtenida mediante la fórmula: $G = \frac{V_{out}}{V_{in\_diff}}$.
3. Explica qué sucede con el voltaje de salida si la fuente de alimentación baja de 12V a 10V.
