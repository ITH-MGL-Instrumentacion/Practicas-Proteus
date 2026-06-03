import serial
import csv
import time

# ---------------------------------------------------------------------------
# Configuración
# ---------------------------------------------------------------------------
PUERTO_COM     = 'COM3'               # Puerto del COMPIM de Proteus o Arduino físico
BAUDIOS        = 9600
ARCHIVO_SALIDA = 'datos_hielera.csv'
TIMEOUT_SILENCIO_S = 30              # Segundos sin datos antes de cerrar solo

# ---------------------------------------------------------------------------
# Conexión y Adquisición
# ---------------------------------------------------------------------------
print(f"Conectando al puerto {PUERTO_COM}...")
try:
    ser = serial.Serial(PUERTO_COM, BAUDIOS, timeout=1)
    time.sleep(2)  # Esperar inicialización del Arduino
    print(f"Conectado. Guardando en '{ARCHIVO_SALIDA}'...")
    print("Presiona Ctrl+C para detener manualmente.\n")

    with open(ARCHIVO_SALIDA, mode='w', newline='') as archivo_csv:
        escritor = csv.writer(archivo_csv)
        
        # Encabezado actualizado con las 4 columnas de variables instrumentales
        escritor.writerow(['Tiempo_s', 'Temperatura_C', 'Humedad_%', 'Estado_Foco'])
        archivo_csv.flush()

        ultimo_dato = time.time()

        while True:
            # Timeout de silencio en caso de desconexión
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

            # Detectar mensaje de fin enviado por el algoritmo del Arduino
            if linea.startswith('FIN,'):
                motivo = linea[4:]
                print(f"\nArduino reportó fin del experimento: {motivo}")
                break

            # Ignorar la línea de cabecera si el Arduino la reenvía al reiniciar
            if linea.startswith('Tiempo'):
                continue

            datos = linea.split(',')
            # Validación de integridad: asegurar las 4 lecturas instrumentales
            if len(datos) == 4:
                escritor.writerow(datos)
                archivo_csv.flush()
                print(f"t={datos[0]:>8} s | T={datos[1]:>6} °C | H={datos[2]:>5} % | Foco={datos[3]}")

except KeyboardInterrupt:
    print("\nCaptura de datos detenida manualmente por el usuario.")
except Exception as e:
    print(f"Error en la comunicación serial: {e}")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Puerto serie cerrado correctamente.")
    print(f"Datos del experimento consolidados en '{ARCHIVO_SALIDA}'.")