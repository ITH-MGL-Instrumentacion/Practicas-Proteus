const int leds[] = {4,5,6,7,8,9,10,11,12,13};

void setup() {
  // Configura todos los pines de la barra como salidas
  for (int i = 0; i < 10; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // Lee el valor del potenciómetro conectado a A0
  int lectura = analogRead(A0);
  // Mapea la lectura (0..1023) a un número de LEDs encendidos (0..10)
  int nivel = map(lectura, 0, 1023, 0, 10);

  // Enciende o apaga cada LED según el nivel calculado
  for (int i = 0; i < 10; i++) {
    if (i < nivel) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }

  delay(50); // Pequeña pausa para evitar parpadeos indeseados
}