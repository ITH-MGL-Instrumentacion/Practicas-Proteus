const uint8_t NTC_PIN = A0;
const uint8_t HEATER_PIN = 5;

const float VREF = 5.0f;
const float ADC_COUNTS = 1023.0f;
const float SERIES_RESISTOR_OHMS = 10000.0f;

// NTCM-10K-B3380
const float NTC_R25_OHMS = 10000.0f;
const float NTC_T25_KELVIN = 298.15f;
const float NTC_BETA_25_50 = 3380.0f;

const uint8_t SAMPLE_COUNT = 16;

// Control didáctico por histéresis.
const float SETPOINT_C = 37.0f;
const float HYSTERESIS_C = 0.5f;

// PWM fijo para representar una potencia promedio simple.
const uint8_t HEATER_PWM_ON = 180;

float readAverageAdc();
float adcToVoltage(float adcValue);
float dividerVoltageToResistance(float outputVoltage);
float resistanceToCelsiusBeta(float resistanceOhms);
void applyBangBangHysteresis(float temperatureCelsius);

bool heaterEnabled = false;

void setup() {
  Serial.begin(9600);
  pinMode(HEATER_PIN, OUTPUT);
  analogWrite(HEATER_PIN, 0);

  Serial.println(F("Incubadora didactica con NTC y control por histeresis"));
  Serial.println(F("Tiempo(ms)\tADC\tVout(V)\tRntc(ohm)\tTemp(C)\tPWM"));
}

void loop() {
  const float adcValue = readAverageAdc();
  const float outputVoltage = adcToVoltage(adcValue);
  const float ntcResistance = dividerVoltageToResistance(outputVoltage);

  if (ntcResistance <= 0.0f) {
    analogWrite(HEATER_PIN, 0);
    Serial.println(F("Lectura fuera de rango"));
    delay(1000);
    return;
  }

  const float temperatureCelsius = resistanceToCelsiusBeta(ntcResistance);
  applyBangBangHysteresis(temperatureCelsius);

  Serial.print(millis());
  Serial.print('\t');
  Serial.print(adcValue, 1);
  Serial.print('\t');
  Serial.print(outputVoltage, 4);
  Serial.print('\t');
  Serial.print(ntcResistance, 1);
  Serial.print('\t');
  Serial.print(temperatureCelsius, 2);
  Serial.print('\t');
  Serial.println(heaterEnabled ? HEATER_PWM_ON : 0);

  delay(500);
}

float readAverageAdc() {
  unsigned long accumulator = 0;

  for (uint8_t sample = 0; sample < SAMPLE_COUNT; ++sample) {
    accumulator += analogRead(NTC_PIN);
    delay(5);
  }

  return static_cast<float>(accumulator) / SAMPLE_COUNT;
}

float adcToVoltage(float adcValue) {
  return (adcValue * VREF) / ADC_COUNTS;
}

float dividerVoltageToResistance(float outputVoltage) {
  // Se asume resistor fijo a 5 V y NTC a GND.
  const float denominator = VREF - outputVoltage;

  if (outputVoltage <= 0.0f || denominator <= 0.0f) {
    return -1.0f;
  }

  return SERIES_RESISTOR_OHMS * outputVoltage / denominator;
}

float resistanceToCelsiusBeta(float resistanceOhms) {
  const float inverseTemperature =
      (1.0f / NTC_T25_KELVIN) +
      (log(resistanceOhms / NTC_R25_OHMS) / NTC_BETA_25_50);

  return (1.0f / inverseTemperature) - 273.15f;
}

void applyBangBangHysteresis(float temperatureCelsius) {
  const float lowerThreshold = SETPOINT_C - HYSTERESIS_C;
  const float upperThreshold = SETPOINT_C + HYSTERESIS_C;

  if (!heaterEnabled && temperatureCelsius < lowerThreshold) {
    heaterEnabled = true;
  } else if (heaterEnabled && temperatureCelsius > upperThreshold) {
    heaterEnabled = false;
  }

  analogWrite(HEATER_PIN, heaterEnabled ? HEATER_PWM_ON : 0);
}