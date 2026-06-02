#include <Arduino.h>
#include <LiquidCrystal.h>

// ---------------------------------------------------------------------------
// Pines
// ---------------------------------------------------------------------------
const int PIN_SENSOR     = A0;  // LM35
const int PIN_MOSFET     = 3;   // Calefactor
const int PIN_VENTILADOR = 5;   // Ventilador
const int PIN_BOTON      = 2;   // Botón

// LCD 16x2 en 4 bits
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// ---------------------------------------------------------------------------
// Parámetros
// ---------------------------------------------------------------------------
const float SETPOINT_C   = 37.7;
const float HISTERESIS_C = 1.0;

enum ModoControl { ON_OFF = 0, HISTERESIS = 1, PID = 2 };
const char* NOMBRE_MODO[] = { "On/Off", "Histeresis", "PID" };
ModoControl modo = ON_OFF;

// PID
const float KP = 10.0;
const float KI = 0.5;
const float KD = 50.0;
const float TS_S = 0.5;

float integral = 0.0;
float errorPrev = 0.0;

// ---------------------------------------------------------------------------
// Funciones auxiliares
// ---------------------------------------------------------------------------
float leerTemperatura() {
  const int N = 10;
  long suma = 0;
  for (int i = 0; i < N; i++) {
    suma += analogRead(PIN_SENSOR);
    delay(2);
  }
  float raw = (float)suma / N;
  float vmV = (raw * 5000.0) / 1023.0;
  return vmV / 10.0;
}

void actualizarLCD(float temp) {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print((char)223); // símbolo °
  lcd.print("C   ");
  lcd.setCursor(0, 1);
  lcd.print("Modo:");
  lcd.print(NOMBRE_MODO[modo]);
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------
void setup() {
  pinMode(PIN_MOSFET, OUTPUT);
  pinMode(PIN_VENTILADOR, OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("Incubadora Arduino");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);

  // Ventilador siempre encendido
  analogWrite(PIN_VENTILADOR, 255);

  // Prueba extendida de lámpara
  Serial.println("Prueba de lámpara: encendida 10s...");
  analogWrite(PIN_MOSFET, 255);
  delay(10000);
  analogWrite(PIN_MOSFET, 0);
  Serial.println("Prueba terminada.");
}

// ---------------------------------------------------------------------------
// Loop
// ---------------------------------------------------------------------------
void loop() {
  float temp = leerTemperatura();
  int pwm = 0;

  switch (modo) {
    case ON_OFF:
      pwm = (temp < SETPOINT_C) ? 255 : 0;
      break;

    case HISTERESIS:
      if (temp < SETPOINT_C - HISTERESIS_C) pwm = 255;
      else if (temp > SETPOINT_C + HISTERESIS_C) pwm = 0;
      break;

    case PID: {
      float error = SETPOINT_C - temp;
      integral += KI * error * TS_S;
      float derivada = KD * (error - errorPrev) / TS_S;
      float salida = KP * error + integral + derivada;
      pwm = constrain((int)salida, 0, 255);
      errorPrev = error;
      break;
    }
  }

  analogWrite(PIN_MOSFET, pwm);
  actualizarLCD(temp);

  Serial.print("Temp: ");
  Serial.print(temp, 2);
  Serial.print(" °C | PWM: ");
  Serial.print(pwm);
  Serial.print(" | Modo: ");
  Serial.println(NOMBRE_MODO[modo]);

  delay(200);
}
