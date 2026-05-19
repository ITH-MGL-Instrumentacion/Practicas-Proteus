#include <Arduino.h>
#include <math.h>

const int ntcPin = A0;
const int pwmPin = 3;

const float beta = 4050.0;
const float r0 = 20000.0;
const float t0 = 298.15;
const float resistorFijo = 20000.0;
const float vcc = 5.0;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  int lectura = analogRead(ntcPin);
  float voltaje = lectura * (vcc / 1023.0);
  float resistenciaNTC = resistorFijo * voltaje / max(0.001f, vcc - voltaje);
  float temperaturaK = 1.0 / ((1.0 / t0) + (1.0 / beta) * log(resistenciaNTC / r0));
  float temperaturaC = temperaturaK - 273.15;
  int pwm = constrain(map((int)temperaturaC, 20, 60, 0, 255), 0, 255);

  analogWrite(pwmPin, pwm);

  Serial.print("ADC: ");
  Serial.print(lectura);
  Serial.print("  R_NTC: ");
  Serial.print(resistenciaNTC);
  Serial.print(" ohm  T: ");
  Serial.print(temperaturaC);
  Serial.println("°C");

  delay(250);
}
