#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ---------------------------------------------------------------------------
// Pines
// ---------------------------------------------------------------------------
const int PIN_SENSOR     = A0;  // LM35: 10 mV/°C, referencia 5 V
const int PIN_MOSFET     = 3;   // Calefactor (PWM)
const int PIN_VENTILADOR = 5;   // Ventilador (corre siempre)
const int PIN_BOTON      = 2;   // Botón para cambiar modo (pull-up interno)
const int PIN_DHT        = 4;   // DHT11: temperatura y humedad ambiental

// ---------------------------------------------------------------------------
// Periféricos
// ---------------------------------------------------------------------------
// LCD I2C 16x2. Dirección típica 0x27; si no enciende probar 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(PIN_DHT, DHT11);

// ---------------------------------------------------------------------------
// Modos de control
// ---------------------------------------------------------------------------
// 0: On/Off puro        — calefactor 255 si T < SP, 0 si T >= SP
// 1: Histéresis         — banda ± HISTERESIS_C alrededor del setpoint
// 2: PID                — forma paralela discreta con anti-windup
enum ModoControl { ON_OFF = 0, HISTERESIS = 1, PID = 2 };
// Etiquetas cortas para la LCD (máx 9 caracteres para que quepan en línea 2)
const char* NOMBRE_MODO[] = { "On/Off", "Histeresis", "PID" };

ModoControl modo = ON_OFF;

// ---------------------------------------------------------------------------
// Parámetros comunes
// ---------------------------------------------------------------------------
const float SETPOINT_C    = 37.7;   // Temperatura objetivo (°C)
const float HISTERESIS_C  = 1.0;    // Semibanda para el modo histéresis (°C)

// ---------------------------------------------------------------------------
// Parámetros PID
// Copiar los valores exactos que entrega la última celda de sintonizador.ipynb
// ---------------------------------------------------------------------------
const float KP = 13.9166;
const float KI = 0.253325;
const float KD = 150.8610;

// Período de muestreo (500 ms es adecuado para la dinámica lenta de la hielera)
const float TS_MS = 500.0;
const float TS_S  = TS_MS / 1000.0;

// Refrescar la LCD cada N muestras para no parpadear tanto
const int LCD_CADA_N = 2;   // cada 2 × 500 ms = 1 s

const float PWM_MIN = 0.0;
const float PWM_MAX = 255.0;

// ---------------------------------------------------------------------------
// Variables de estado
// ---------------------------------------------------------------------------
float integral  = 0.0;
float errorPrev = 0.0;

// Estado interno del modo Histéresis
bool calefactorEncendido = false;

unsigned long tiempoInicioMs   = 0;
unsigned long ultimoMuestreoMs = 0;
int contadorLCD = 0;

// Debounce del botón
unsigned long ultimoBotonMs = 0;
const unsigned long DEBOUNCE_MS = 300;
bool estadoBotonPrev = HIGH;

// ---------------------------------------------------------------------------
// Funciones auxiliares
// ---------------------------------------------------------------------------
float leerTemperatura() {
  int   raw = analogRead(PIN_SENSOR);
  float vmV = (raw * 5000.0) / 1023.0;
  return vmV / 10.0;  // LM35: 10 mV/°C
}

void resetPID() {
  integral  = 0.0;
  errorPrev = 0.0;
}

// Muestra en LCD:
//   Línea 0: "T:XX.X°C H:XX%  "
//   Línea 1: "Modo: XXXXXXXXXX"
void actualizarLCD(float tempC, float humPct) {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC, 1);
  lcd.print("\xDF""C ");          // carácter "°" en ROM del HD44780
  lcd.print("H:");
  if (!isnan(humPct)) {
    lcd.print((int)humPct);
    lcd.print("%  ");
  } else {
    lcd.print("--% ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Modo:");
  lcd.print(NOMBRE_MODO[modo]);
  // Rellenar con espacios para borrar texto anterior
  for (int i = 5 + strlen(NOMBRE_MODO[modo]); i < 16; i++) lcd.print(' ');
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------
void setup() {
  pinMode(PIN_MOSFET, OUTPUT);
  analogWrite(PIN_MOSFET, 0);

  pinMode(PIN_VENTILADOR, OUTPUT);
  analogWrite(PIN_VENTILADOR, 255);  // Ventilador al máximo siempre

  pinMode(PIN_BOTON, INPUT_PULLUP);  // Botón entre D2 y GND

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Control hielera");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  // DHT11
  dht.begin();

  delay(1500);
  lcd.clear();

  tiempoInicioMs   = millis();
  ultimoMuestreoMs = tiempoInicioMs;
}

// ---------------------------------------------------------------------------
// Loop principal
// ---------------------------------------------------------------------------
void loop() {
  unsigned long ahora = millis();

  // --- Detección del botón (flanco de bajada + debounce) ---
  bool estadoBoton = digitalRead(PIN_BOTON);
  if (estadoBotonPrev == HIGH && estadoBoton == LOW &&
      (ahora - ultimoBotonMs) > DEBOUNCE_MS) {

    ultimoBotonMs = ahora;
    modo = (ModoControl)((modo + 1) % 3);
    resetPID();
    calefactorEncendido = false;
    analogWrite(PIN_MOSFET, 0);
    contadorLCD = LCD_CADA_N;  // forzar refresco inmediato
  }
  estadoBotonPrev = estadoBoton;

  // --- Esperar el siguiente período de muestreo ---
  if (ahora - ultimoMuestreoMs < (unsigned long)TS_MS) return;
  ultimoMuestreoMs = ahora;

  float temp   = leerTemperatura();
  float humPct = dht.readHumidity();   // NaN si falla la lectura
  int   pwm    = 0;

  // --- Calcular salida según el modo activo ---
  switch (modo) {

    case ON_OFF:
      pwm = (temp < SETPOINT_C) ? 255 : 0;
      break;

    case HISTERESIS:
      if (temp < SETPOINT_C - HISTERESIS_C) calefactorEncendido = true;
      if (temp > SETPOINT_C + HISTERESIS_C) calefactorEncendido = false;
      pwm = calefactorEncendido ? 255 : 0;
      break;

    case PID: {
      float error = SETPOINT_C - temp;

      float integralNueva = integral + KI * error * TS_S;
      float termD         = KD * (error - errorPrev) / TS_S;
      float u             = KP * error + integralNueva + termD;

      if (u >= PWM_MIN && u <= PWM_MAX) integral = integralNueva;

      pwm       = (int)constrain(u, PWM_MIN, PWM_MAX);
      errorPrev = error;
      break;
    }
  }

  analogWrite(PIN_MOSFET, pwm);

  // --- Refrescar LCD (cada LCD_CADA_N muestras) ---
  if (++contadorLCD >= LCD_CADA_N) {
    contadorLCD = 0;
    actualizarLCD(temp, humPct);
  }
}
