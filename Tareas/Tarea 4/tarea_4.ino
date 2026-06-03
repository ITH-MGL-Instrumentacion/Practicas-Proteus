/*
  Entrada_Analogica.ino
  Objetivo: Leer señal analógica en A0 y representarla mediante:
            1. Binario puro de 10 bits (Práctica Base)
            2. Barra de nivel/creciente (Práctica Adicional 1)
            3. Control de brillo por PWM en D3 (Práctica Adicional 2)
*/

// --- Configuración de Pines ---
const int pinPot = A0;      // Pin del potenciómetro
const int pinPWM = 3;       // Pin para el LED con brillo variable (PWM)

// Arreglo de pines para los 10 LEDs (Bit 0 al Bit 9)
// LED en pin 4 = Bit 0 (LSB) -> LED en pin 13 = Bit 9 (MSB)
const int pinesLEDs[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; 
const int totalLEDs = 10;

void setup() {
  // Configurar los pines de la barra de LEDs como salidas
  for (int i = 0; i < totalLEDs; i++) {
    pinMode(pinesLEDs[i], OUTPUT);
  }
  
  // Configurar el pin PWM como salida
  pinMode(pinPWM, OUTPUT);
}

void loop() {
  // 1. Leer el valor del ADC (0 a 1023)
  int lecturaADC = analogRead(pinPot);

  // 2. [Práctica Adicional 2] - Controlar el brillo del LED en D3
  // Convertimos el rango del ADC (0-1023) al rango PWM (0-255)
  int valorPWM = map(lecturaADC, 0, 1023, 0, 255);
  analogWrite(pinPWM, valorPWM);

  // =================================================================
  // SELECCIÓN DE MODO VISUAL PARA LOS 10 LEDs
  // Descomenta la función que quieras probar y comenta la otra.
  // =================================================================
  
  mostrarBinario(lecturaADC);       // Práctica Base (Binario de 10 bits)
  // mostrarBarraNivel(lecturaADC); // Práctica Adicional 1 (Barra creciente)

  // Pequeña pausa para estabilidad visual y evitar parpadeos
  delay(15); 
}

// --- Función Práctica Base: Conversión A/D Directa a Binario ---
void mostrarBinario(int lectura) {
  for (int bit = 0; bit < totalLEDs; bit++) {
    // Desplazar a la derecha y aislar el último bit
    int estadoBit = (lectura >> bit) & 0x01;
    // Escribir en el LED correspondiente
    digitalWrite(pinesLEDs[bit], estadoBit);
  }
}

// --- Función Práctica Adicional 1: Barra de Nivel Creciente ---
void mostrarBarraNivel(int lectura) {
  // Convertir el rango del ADC (0-1023) a cantidad de LEDs encendidos (0-10)
  int cantidadEncendida = map(lectura, 0, 1023, 0, totalLEDs);
  
  for (int i = 0; i < totalLEDs; i++) {
    if (i < cantidadEncendida) {
      digitalWrite(pinesLEDs[i], HIGH); // Enciende si está dentro del nivel
    } else {
      digitalWrite(pinesLEDs[i], LOW);  // Apaga el resto
    }
  }
}
