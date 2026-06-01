#include <Arduino.h>

// ---------------------------------------------------------------------------
// Pines
// ---------------------------------------------------------------------------
const int PIN_SENSOR    = A0;  // Sensor de temperatura del OVEN (10 mV/°C)
const int PIN_MOSFET    = 3;   // Salida PWM hacia el IRLZ44N
const int PIN_VENTILADOR = 5;  // Ventilador (hardware real — ignorado en Proteus)

// ---------------------------------------------------------------------------
// Parámetros del experimento
// ---------------------------------------------------------------------------
const float TIEMPO_ESCALON_S   = 5.0;    // Segundos hasta aplicar el escalón
const float TIEMPO_MAX_S       = 900.0;  // Tiempo máximo del experimento (seg) — 15 min
const float TEMP_MAX_C         = 85.0;   // Temperatura de corte de seguridad (poliestirenо ~80 °C)
const int   PWM_ESCALON        = 128;    // Escalón al 50 % (128/255) — seguro para la hielera

// Detección de estado estacionario
// Ventana más larga y umbral más bajo: la curva a 50 % PWM sube muy despacio
// cerca del régimen permanente y 5 s no es suficiente para decidir.
const int   VENTANA_SS  = 150;    // 150 muestras × 100 ms = 15 s de ventana
const float UMBRAL_SS   = 0.15;   // Variación máxima tolerable en la ventana (°C)

// ---------------------------------------------------------------------------
// Variables de estado
// ---------------------------------------------------------------------------
unsigned long tiempoInicio = 0;
bool escalonAplicado = false;
bool experimentoTerminado = false;

float bufferTemp[VENTANA_SS];
int   indiceBuffer = 0;
bool  bufferLleno  = false;

void setup() {
  pinMode(PIN_MOSFET, OUTPUT);
  analogWrite(PIN_MOSFET, 0);

  pinMode(PIN_VENTILADOR, OUTPUT);
  analogWrite(PIN_VENTILADOR, 255);  // Ventilador al máximo desde el inicio

  Serial.begin(9600);
  Serial.println("Tiempo_s,Temperatura_C,Entrada_PWM");

  // Inicializar buffer de estado estacionario
  for (int i = 0; i < VENTANA_SS; i++) bufferTemp[i] = 0.0;

  tiempoInicio = millis();
}

void apagarYTerminar(const char* motivo) {
  analogWrite(PIN_MOSFET, 0);
  analogWrite(PIN_VENTILADOR, 0);  // Apagar ventilador al terminar
  experimentoTerminado = true;
  Serial.print("FIN,");
  Serial.println(motivo);
}

bool estadoEstacionario(float tempActual) {
  bufferTemp[indiceBuffer] = tempActual;
  indiceBuffer = (indiceBuffer + 1) % VENTANA_SS;
  if (indiceBuffer == 0) bufferLleno = true;
  if (!bufferLleno) return false;

  float tMin = bufferTemp[0], tMax = bufferTemp[0];
  for (int i = 1; i < VENTANA_SS; i++) {
    if (bufferTemp[i] < tMin) tMin = bufferTemp[i];
    if (bufferTemp[i] > tMax) tMax = bufferTemp[i];
  }
  return (tMax - tMin) < UMBRAL_SS;
}

void loop() {
  if (experimentoTerminado) return;

  unsigned long tiempoActualMs = millis() - tiempoInicio;
  float tiempoSegundos = tiempoActualMs / 1000.0;

  // Leer temperatura del OVEN (10 mV/°C, referencia 5 V)
  int   lecturaADC  = analogRead(PIN_SENSOR);
  float voltajemV   = (lecturaADC * 5000.0) / 1023.0;
  float temperaturaC = voltajemV / 10.0;

  // --- Corte por temperatura máxima ---
  if (temperaturaC >= TEMP_MAX_C) {
    apagarYTerminar("TEMP_MAX");
    return;
  }

  // --- Corte por tiempo máximo ---
  if (tiempoSegundos >= TIEMPO_MAX_S) {
    apagarYTerminar("TIEMPO_MAX");
    return;
  }

  // --- Aplicar escalón ---
  int pwmActual = 0;
  if (tiempoSegundos >= TIEMPO_ESCALON_S) {
    pwmActual = PWM_ESCALON;
    if (!escalonAplicado) escalonAplicado = true;
  }
  analogWrite(PIN_MOSFET, pwmActual);

  // --- Detección de estado estacionario (solo tras el escalón) ---
  if (escalonAplicado && estadoEstacionario(temperaturaC)) {
    // Enviar la muestra actual antes de terminar
    Serial.print(tiempoSegundos, 2); Serial.print(",");
    Serial.print(temperaturaC, 2);   Serial.print(",");
    Serial.println(pwmActual);
    apagarYTerminar("ESTADO_ESTACIONARIO");
    return;
  }

  // --- Enviar muestra en formato CSV ---
  Serial.print(tiempoSegundos, 2); Serial.print(",");
  Serial.print(temperaturaC, 2);   Serial.print(",");
  Serial.println(pwmActual);

  delay(100); // Muestreo cada 100 ms
}