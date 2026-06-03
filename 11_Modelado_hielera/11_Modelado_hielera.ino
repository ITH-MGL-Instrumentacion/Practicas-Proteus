#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Configuración LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- Configuración Sensor de Humedad ---
const int PIN_DHT = 2;
#define DHTTYPE DHT22   // Cambiar a DHT11 si tu sensor físico es el azul
DHT dht(PIN_DHT, DHTTYPE);

// ---------------------------------------------------------------------------
// Pines de Hardware
// ---------------------------------------------------------------------------
const int PIN_SENSOR     = A0;  // Termistor NTC 10k
const int PIN_FOCO       = 3;   // Relevador del foco (Lógica Invertida)
const int PIN_VENTILADOR = 5;   // Relevador del ventilador (Lógica Invertida)

// ---------------------------------------------------------------------------
// Parámetros del Termistor NTC 10k B3950
// ---------------------------------------------------------------------------
const float R_SERIE           = 10000.0; 
const float TERMISTOR_NOMINAL = 10000.0; 
const float TEMP_NOMINAL      = 25.0;    
const float B_COEFICIENTE     = 3950.0;  

// ---------------------------------------------------------------------------
// Parámetros del Experimento (Respuesta al Escalón)
// ---------------------------------------------------------------------------
const float TIEMPO_ESCALON_S   = 5.0;   
const float TIEMPO_MAX_S       = 900.0; 
const float TEMP_MAX_C         = 85.0;  

// Detección de Estado Estacionario
const int   VENTANA_SS         = 150;   
const float UMBRAL_SS          = 0.15;  

// ---------------------------------------------------------------------------
// Variables de Estado y Temporizadores
// ---------------------------------------------------------------------------
unsigned long tiempoInicio = 0;
bool escalonAplicado = false;
bool experimentoTerminado = false;

float bufferTemp[VENTANA_SS];
int   indiceBuffer = 0;
bool  bufferLleno  = false;

unsigned long tiempoUltimaLCD = 0;
unsigned long tiempoUltimoDHT = 0; // Temporizador para no saturar el DHT22
float humedadActual = 0.0;         // Almacena la última humedad válida

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando OVEN...");

  dht.begin();

  // Configuración del Foco (Inicia APAGADO con HIGH por lógica invertida)
  pinMode(PIN_FOCO, OUTPUT);
  digitalWrite(PIN_FOCO, HIGH); 
  
  // Configuración del Ventilador 
  pinMode(PIN_VENTILADOR, OUTPUT);
  // Se envía LOW para activar el relevador de lógica invertida desde el inicio
  digitalWrite(PIN_VENTILADOR, LOW); 

  Serial.begin(9600);
  Serial.println("Tiempo_s,Temperatura_C,Humedad_%,Estado_Foco");

  for (int i = 0; i < VENTANA_SS; i++) bufferTemp[i] = 0.0;
  
  tiempoInicio = millis();
}

void apagarYTerminar(const char* motivo) {
  // Estado de seguridad (Ambos relevadores apagados con HIGH)
  digitalWrite(PIN_FOCO, HIGH);       
  digitalWrite(PIN_VENTILADOR, HIGH);  
  
  experimentoTerminado = true;
  
  lcd.clear();
  lcd.setCursor(0, 0);
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

  // --- 1. Procesamiento matemático del Termistor NTC (Cada 100ms) ---
  int lecturaADC = analogRead(PIN_SENSOR);
  if (lecturaADC == 0) lecturaADC = 1; 
  if (lecturaADC == 1023) lecturaADC = 1022;

  float resistenciaNTC = R_SERIE * ((float)lecturaADC / (1023.0 - lecturaADC));
  float temperaturaC = resistenciaNTC / TERMISTOR_NOMINAL;     
  temperaturaC = log(temperaturaC);                            
  temperaturaC /= B_COEFICIENTE;                               
  temperaturaC += 1.0 / (TEMP_NOMINAL + 273.15);               
  temperaturaC = 1.0 / temperaturaC;                           
  temperaturaC -= 273.15;                                      

  // --- 2. Lectura Protegida del DHT22 (Solo cada 2000ms) ---
  if (millis() - tiempoUltimoDHT >= 2000) {
    float lecturaHumedad = dht.readHumidity();
    if (!isnan(lecturaHumedad)) {
      humedadActual = lecturaHumedad; 
    }
    tiempoUltimoDHT = millis();
  }

  // --- 3. Límites de Control de Seguridad ---
  if (temperaturaC >= TEMP_MAX_C || tiempoSegundos >= TIEMPO_MAX_S) {
    apagarYTerminar(temperaturaC >= TEMP_MAX_C ? "TEMP_MAX" : "TIEMPO_MAX");
    return;
  }

  // --- 4. Aplicación del Escalón ---
  bool logicaFocoDeseada = false; 
  if (tiempoSegundos >= TIEMPO_ESCALON_S) {
    logicaFocoDeseada = true; 
    if (!escalonAplicado) escalonAplicado = true;
  }
  
  // Control físico del Foco (LOW = Encendido, HIGH = Apagado)
  digitalWrite(PIN_FOCO, logicaFocoDeseada ? LOW : HIGH);

  // --- 5. Interfaz Visual LCD (Cada 500ms) ---
  if (millis() - tiempoUltimaLCD >= 500) {
    lcd.setCursor(0, 0);
    lcd.print("T: "); lcd.print(temperaturaC, 1); lcd.print("C ");
    
    if (humedadActual > 0.0) {
      lcd.print("H: "); lcd.print(humedadActual, 0); lcd.print("%");
    } else {
      lcd.print("H:--%");
    }
    
    lcd.setCursor(0, 1);
    lcd.print("Foco: "); 
    lcd.print(logicaFocoDeseada ? "ON (CONST) " : "OFF        ");
    tiempoUltimaLCD = millis();
  }

  // --- 6. Monitoreo de Estado Estacionario ---
  if (escalonAplicado && estadoEstacionario(temperaturaC)) {
    Serial.print(tiempoSegundos, 2); Serial.print(",");
    Serial.print(temperaturaC, 2);   Serial.print(",");
    Serial.print(humedadActual, 1);  Serial.print(",");
    Serial.println(logicaFocoDeseada ? 1 : 0);
    
    apagarYTerminar("ESTADO_ESTACIONARIO");
    return;
  }

  // --- 7. Envío de Datos por Puerto Serie ---
  Serial.print(tiempoSegundos, 2); Serial.print(",");
  Serial.print(temperaturaC, 2);   Serial.print(",");
  Serial.print(humedadActual, 1);  Serial.print(",");
  Serial.println(logicaFocoDeseada ? 1 : 0); 

  delay(100); 
}
// === FIN DEL CODIGO (ASEGURATE DE COPIAR HASTA AQUI) ===
