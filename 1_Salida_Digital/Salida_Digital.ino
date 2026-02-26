#include <stdint.h>
#include <Arduino.h>

uint8_t ledPin1 = 3;
uint8_t ledPin2 = 5;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(300);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  delay(700);
}