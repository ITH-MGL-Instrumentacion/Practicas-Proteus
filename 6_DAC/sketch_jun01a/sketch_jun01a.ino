#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Configuración LCD: Dirección 0x27 o 0x3F, 16 columnas, 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------------------------------------------------------------------
// Pines y Parámetros (iguales a tu configuración)
// ---------------------------------------------------------------------------
const int PIN_SENSOR     = A0;
const int PIN_MOSFET     = 3;
const int PIN_VENTILADOR = 5;

const float TIEMPO_ESCALON_S   = 5.0;
const float TIEMPO_MAX_S       = 900.0;
const float TEMP_MAX_C         = 85.0;
const int   PWM_ESCALON        = 128;

const int   VENTANA_SS  = 150;
const float UMBRAL_SS   = 0.15;

// Variables de estado
unsigned long tiempoInicio = 0;
bool escalonAplicado = false;
bool experimentoTerminado = false;

float bufferTemp[VENTANA_SS];
int   indiceBuffer = 0;
bool  bufferLleno  = false;

// Variable para refresco de LCD
unsigned long ultimaActualizacionLCD = 0;

void setup() {
  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");

  pinMode(PIN_MOSFET, OUTPUT);
  analogWrite(PIN_MOSFET, 0);
  pinMode(PIN_VENTILADOR, OUTPUT);
  analogWrite(PIN_VENTILADOR, 255);

  Serial.begin(9600);
  Serial.println("Tiempo_s,Temperatura_C,Entrada_PWM");

  for (int i = 0; i < VENTANA_SS; i++) bufferTemp[i] = 0.0;
  tiempoInicio = millis();
}

void apagarYTerminar(const char* motivo) {
  analogWrite(PIN_MOSFET, 0);
  analogWrite(PIN_VENTILADOR, 0);
  experimentoTerminado = true;
  
  lcd.clear();
  lcd.print("FIN EXPERIMENTO");
  lcd.setCursor(0, 1);
  lcd.print(motivo);
  
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

  int   lecturaADC  = analogRead(PIN_SENSOR);
  float voltajemV   = (lecturaADC * 5000.0) / 1023.0;
  float temperaturaC = voltajemV / 10.0;

  // --- Corte ---
  if (temperaturaC >= TEMP_MAX_C || tiempoSegundos >= TIEMPO_MAX_S) {
    apagarYTerminar(temperaturaC >= TEMP_MAX_C ? "TEMP_MAX" : "TIEMPO_MAX");
    return;
  }

  // --- Lógica PWM ---
  int pwmActual = (tiempoSegundos >= TIEMPO_ESCALON_S) ? PWM_ESCALON : 0;
  analogWrite(PIN_MOSFET, pwmActual);

  // --- Refresco LCD (cada 1000ms para evitar parpadeo) ---
  if (millis() - ultimaActualizacionLCD > 1000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: "); lcd.print(temperaturaC, 1); lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("PWM: "); lcd.print(pwmActual);
    ultimaActualizacionLCD = millis();
  }

  // --- Detección y Serial ---
  if (pwmActual > 0 && estadoEstacionario(temperaturaC)) {
    apagarYTerminar("ESTADO_ESTACIONARIO");
    return;
  }

  Serial.print(tiempoSegundos, 2); Serial.print(",");
  Serial.print(temperaturaC, 2);   Serial.print(",");
  Serial.println(pwmActual);

  delay(100);
}
