import serial
import csv
import time

# ---------------------------------------------------------------------------
# Configuración
# ---------------------------------------------------------------------------
PUERTO_COM     = 'COM6'               # Puerto del COMPIM de Proteus o Arduino físico
BAUDIOS        = 9600
ARCHIVO_SALIDA = 'datos_hielera.csv'
TIMEOUT_SILENCIO_S = 30              # Segundos sin datos antes de cerrar solo
PUERTO_ESPERA_MAX_S = 10             # Segundos máximos para esperar al puerto serie
REINTENTO_PUERTO_S = 0.5             # Intervalo entre intentos de apertura

# ---------------------------------------------------------------------------
# Conexión
# ---------------------------------------------------------------------------

def abrir_puerto(com, baud, timeout_s=10, reintento_s=0.5, espera_max_s=10):
    inicio = time.time()
    while True:
        try:
            puerto = serial.Serial(com, baud, timeout=timeout_s)
            return puerto
        except serial.SerialException:
            if time.time() - inicio >= espera_max_s:
                raise
            time.sleep(reintento_s)

print(f"Conectando al puerto {PUERTO_COM}...")
try:
    ser = abrir_puerto(PUERTO_COM, BAUDIOS, timeout_s=10, reintento_s=REINTENTO_PUERTO_S, espera_max_s=PUERTO_ESPERA_MAX_S)
    time.sleep(2)  # Esperar inicialización del Arduino
    print(f"Conectado. Guardando en '{ARCHIVO_SALIDA}'...")
    print("Presiona Ctrl+C para detener manualmente.\n")

    with open(ARCHIVO_SALIDA, mode='w', newline='') as archivo_csv:
        escritor = csv.writer(archivo_csv)
        # Encabezado fijo en el CSV
        escritor.writerow(['Tiempo_s', 'Temperatura_C', 'Entrada_PWM'])
        archivo_csv.flush()

        ultimo_dato = time.time()

        while True:
            # Timeout de silencio
            if time.time() - ultimo_dato > TIMEOUT_SILENCIO_S:
                print(f"\nSin datos durante {TIMEOUT_SILENCIO_S} s. Cerrando.")
                break

            linea_raw = ser.readline()
            if not linea_raw:
                continue

            linea = linea_raw.decode('utf-8', errors='replace').strip()
            if not linea:
                continue

            ultimo_dato = time.time()

            # Detectar mensaje de fin enviado por el Arduino
            if linea.startswith('FIN,'):
                motivo = linea[4:]
                print(f"\nArduino reportó fin del experimento: {motivo}")
                break

            # Ignorar la línea de cabecera si el Arduino la reenvía
            if linea.startswith('Tiempo'):
                continue

            datos = linea.split(',')
            if len(datos) == 3:
                escritor.writerow(datos)
                archivo_csv.flush()
                print(f"t={datos[0]:>8} s | T={datos[1]:>6} °C | PWM={datos[2]}")

except KeyboardInterrupt:
    print("\nCaptura detenida por el usuario.")
except Exception as e:
    print(f"Error: {e}")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Puerto serie cerrado.")
    print(f"Datos guardados en '{ARCHIVO_SALIDA}'.")