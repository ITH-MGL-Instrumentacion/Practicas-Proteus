# Barra creciente/decreciente con 10 LEDs

En esta práctica adicional no se visualiza un número binario, sino una barra de nivel formada por diez LEDs. Al mover un potenciómetro conectado a la entrada analógica `A0` se aumenta o disminuye la cantidad de LEDs encendidos.

## Algoritmo

1. Definir arreglo con los pines de los 10 LEDs (`4..13`).
2. En `setup()` configurar cada pin como salida.
3. En el `loop()`:
   - Leer el valor analógico de `A0` (`0..1023`).
   - Convertir la lectura al número de LEDs encendidos (`0..10`) usando `map()`.
   - Recorrer los 10 LEDs con un `for` y:
     * si el índice es menor que la cantidad calculada, encender el LED;
     * en caso contrario apagarlo.

## Código Arduino

```cpp
const int leds[] = {4,5,6,7,8,9,10,11,12,13};

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  int lectura = analogRead(A0);
  int nivel = map(lectura, 0, 1023, 0, 10);
  for (int i = 0; i < 10; i++) {
    if (i < nivel) {
      digitalWrite(leds[i], HIGH);
    } else {
      digitalWrite(leds[i], LOW);
    }
  }
  delay(50);
}
```

### Funciones utilizadas

- `analogRead()`: obtiene la lectura del potenciómetro.
- `map()`: ajusta la lectura al rango de LEDs a encender.
- `for`: recorre los diez pines de la barra.
- `digitalWrite()`: enciende o apaga cada LED.

## Archivos

- Código Arduino: [Barra_LED.ino](Barra_LED.ino)

---

| Anterior | Índice | Siguiente |
|---|---|---|
| [Entrada analógica](../4_Entrada_Analogica/Entrada_Analogica.md) | [Volver al índice](../README.md#prácticas-básicas) | [CDA R-2R](../7_CDA_R_2R/CDA_R_2R.md) |
