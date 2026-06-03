#include <Arduino.h>

// ---------------------------------------------------------------------------
// Pines
// ---------------------------------------------------------------------------
const int PIN_SENSOR     = A0;  // Sensor de temperatura (LM35: 10 mV/°C, ref 5 V)
const int PIN_MOSFET     = 3;   // Salida PWM hacia el IRLZ44N
const int PIN_VENTILADOR = 5;   // Ventilador (hardware real — ignorado en Proteus)

// ---------------------------------------------------------------------------
// Parámetros del método del relé (auto-tune PID por histéresis)
// ---------------------------------------------------------------------------
// Setpoint de calibración — elígelo dentro del rango de trabajo normal.
const float SETPOINT_C    = 37.7;  // Temperatura objetivo de la calibración

// Banda de histéresis. El relé se enciende cuando T < SP - HISTERESIS
// y se apaga cuando T > SP + HISTERESIS. Valores típicos: 1 °C a 3 °C.
const float HISTERESIS_C  = 2.0;

// PWM del relé cuando está encendido. Debe coincidir con el PWM_ESCALON usado
// en la práctica de identificación para que Ku y Kp sean comparables.
const int   PWM_ON        = 128;  // 50 % — igual que el escalón del sintonizador

// Número de ciclos completos a promediar (mínimo recomendado: 3).
// Se descarten los primeros CICLOS_DESCARTE ciclos para evitar transitorios.
const int   CICLOS_UTILES    = 4;
const int   CICLOS_DESCARTE  = 2;

// Tiempo máximo de seguridad para no dejar el calefactor encendido indefinidamente.
const float TIEMPO_MAX_S  = 1200.0;  // 20 minutos
const float TEMP_MAX_C    = 55.0;    // Corte de emergencia

// ---------------------------------------------------------------------------
// Estado del relé y detección de ciclos
// ---------------------------------------------------------------------------
enum Estado { ESPERANDO, CORRIENDO, LISTO };
Estado estado = ESPERANDO;

bool releEncendido = false;

// Detección de picos y valles ---
// Se registra la temperatura máxima y mínima dentro de cada medio-ciclo.
float tPicoActual    = -999.0;
float tValleActual   =  999.0;

// Tiempos de los cruces del setpoint (para medir período)
unsigned long tCruceAnteriorMs = 0;
unsigned long tCruceActualMs   = 0;
bool esperandoPrimerCruce = true;

// Acumuladores para el promedio
float sumaPeriodosMs = 0.0;  // suma de períodos en ms
float sumaAmplitudes = 0.0;  // suma de semiamplitudes (°C)
int   ciclosRecogidos = 0;
int   ciclosTotales   = 0;   // incluye los de descarte

// Temperatura previa para detectar cambio de estado del relé
float tempAnterior = 0.0;
bool  mitadCicloAscendente = false;  // true = estamos en la mitad de calentamiento

unsigned long tiempoInicioMs = 0;

// ---------------------------------------------------------------------------
// Lectura de temperatura
// ---------------------------------------------------------------------------
float leerTemperatura() {
  int   raw  = analogRead(PIN_SENSOR);
  float vmV  = (raw * 5000.0) / 1023.0;
  return vmV / 10.0;  // LM35: 10 mV/°C
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------
void setup() {
  pinMode(PIN_MOSFET, OUTPUT);
  analogWrite(PIN_MOSFET, 0);

  pinMode(PIN_VENTILADOR, OUTPUT);
  analogWrite(PIN_VENTILADOR, 255);  // Ventilador al máximo durante calibración

  Serial.begin(9600);

  Serial.println("# Autotune PID por metodo del rele (Astrom-Hagglund)");
  Serial.print("# Setpoint: "); Serial.print(SETPOINT_C, 1); Serial.println(" C");
  Serial.print("# Histeresis: +/-"); Serial.print(HISTERESIS_C, 1); Serial.println(" C");
  Serial.print("# PWM_ON: "); Serial.println(PWM_ON);
  Serial.println("# Esperando que la temperatura alcance la zona del setpoint...");
  Serial.println("Tiempo_s,Temperatura_C,Rele");

  tiempoInicioMs = millis();
}

// ---------------------------------------------------------------------------
// Fin del experimento: imprimir resultados y apagar
// ---------------------------------------------------------------------------
void calcularYTerminar() {
  analogWrite(PIN_MOSFET, 0);
  analogWrite(PIN_VENTILADOR, 0);

  float Tu_s = (sumaPeriodosMs / ciclosRecogidos) / 1000.0;
  float a    = sumaAmplitudes  / ciclosRecogidos;

  // Relé asimétrico (0 a PWM_ON): amplitud equivalente d = PWM_ON/2
  float d    = PWM_ON / 2.0;
  float Ku   = (4.0 * d) / (PI * a);

  // Sintonía Ziegler-Nichols a partir del relé
  float Kp = 0.6  * Ku;
  float Ki = 1.2  * Ku / Tu_s;
  float Kd = 0.075 * Ku * Tu_s;

  Serial.println();
  Serial.println("# ============================================================");
  Serial.println("# RESULTADOS DEL AUTOTUNE");
  Serial.println("# ============================================================");
  Serial.print("# Ciclos promediados : "); Serial.println(ciclosRecogidos);
  Serial.print("# Periodo Tu         : "); Serial.print(Tu_s, 3); Serial.println(" s");
  Serial.print("# Semiamplitud a     : "); Serial.print(a, 4);   Serial.println(" C");
  Serial.print("# Ganancia critica Ku: "); Serial.println(Ku, 4);
  Serial.println("# ---");
  Serial.println("# Parametros PID (Ziegler-Nichols / rele):");
  Serial.print("# Kp = "); Serial.println(Kp, 4);
  Serial.print("# Ki = "); Serial.println(Ki, 6);
  Serial.print("# Kd = "); Serial.println(Kd, 4);
  Serial.println("# ============================================================");
  Serial.println("FIN,AUTOTUNE_OK");
}

void terminarError(const char* motivo) {
  analogWrite(PIN_MOSFET, 0);
  analogWrite(PIN_VENTILADOR, 0);
  Serial.print("FIN,ERROR_");
  Serial.println(motivo);
}

// ---------------------------------------------------------------------------
// Loop principal
// ---------------------------------------------------------------------------
void loop() {
  static bool terminado = false;
  if (terminado) return;

  unsigned long ahora    = millis();
  float tiempoS          = (ahora - tiempoInicioMs) / 1000.0;
  float temp             = leerTemperatura();

  // --- Cortes de seguridad ---
  if (temp >= TEMP_MAX_C) {
    terminarError("TEMP_MAX");
    terminado = true;
    return;
  }
  if (tiempoS >= TIEMPO_MAX_S) {
    terminarError("TIEMPO_MAX");
    terminado = true;
    return;
  }

  // =========================================================================
  // MÁQUINA DE ESTADOS
  // =========================================================================

  if (estado == ESPERANDO) {
    // Calentar a máxima potencia hasta entrar en la banda.
    // Así el primer ciclo ya parte desde una condición próxima al régimen.
    analogWrite(PIN_MOSFET, PWM_ON);

    if (temp >= SETPOINT_C - HISTERESIS_C * 2.0) {
      estado = CORRIENDO;
      releEncendido       = (temp < SETPOINT_C);
      mitadCicloAscendente = releEncendido;
      tPicoActual  = temp;
      tValleActual = temp;
      Serial.println("# Iniciando ciclos del rele...");
    }
  }

  else if (estado == CORRIENDO) {

    // --- Lógica del relé ---
    if (!releEncendido && temp < SETPOINT_C - HISTERESIS_C) {
      // Cruce hacia abajo: encender calefactor
      releEncendido = true;

      // Registrar el valle del medio-ciclo que acaba de terminar
      unsigned long tCruceMs = ahora;

      if (!esperandoPrimerCruce) {
        // Cada cruce de encendido cierra un ciclo completo (encendido → apagado → encendido)
        // Solo se cuenta el período entre dos cruces del mismo tipo (encendido→encendido)
        unsigned long periodoMs = tCruceMs - tCruceAnteriorMs;

        ciclosTotales++;
        if (ciclosTotales > CICLOS_DESCARTE) {
          // Semiamplitud del ciclo recién cerrado:
          // pico fue el máximo del medio-ciclo de calentamiento,
          // valle es el mínimo del medio-ciclo de enfriamiento que acaba.
          float semiAmplitud = (tPicoActual - tValleActual) / 2.0;
          sumaPeriodosMs += periodoMs;
          sumaAmplitudes += semiAmplitud;
          ciclosRecogidos++;

          Serial.print("# Ciclo "); Serial.print(ciclosTotales);
          Serial.print("  Tu="); Serial.print(periodoMs / 1000.0, 2);
          Serial.print(" s  a="); Serial.print(semiAmplitud, 3);
          Serial.println(" C");

          if (ciclosRecogidos >= CICLOS_UTILES) {
            calcularYTerminar();
            terminado = true;
            return;
          }
        }
      }
      // Reiniciar seguimiento para el nuevo medio-ciclo ascendente
      tCruceAnteriorMs     = tCruceMs;
      esperandoPrimerCruce = false;
      tPicoActual          = temp;
      tValleActual         = temp;
    }

    else if (releEncendido && temp > SETPOINT_C + HISTERESIS_C) {
      // Cruce hacia arriba: apagar calefactor
      releEncendido = false;
      // Reiniciar seguimiento para el medio-ciclo descendente
      tPicoActual  = temp;
      tValleActual = temp;
    }

    // Actualizar extremos del medio-ciclo en curso
    if (temp > tPicoActual)  tPicoActual  = temp;
    if (temp < tValleActual) tValleActual = temp;

    analogWrite(PIN_MOSFET, releEncendido ? PWM_ON : 0);
  }

  // --- Imprimir muestra CSV ---
  Serial.print(tiempoS, 2);  Serial.print(",");
  Serial.print(temp,    2);  Serial.print(",");
  Serial.println(releEncendido ? PWM_ON : 0);

  tempAnterior = temp;
  delay(200);  // Muestreo cada 200 ms — suficiente para oscilaciones térmicas lentas
}
