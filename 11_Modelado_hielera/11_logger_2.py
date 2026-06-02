import serial
import csv
import time

# ---------------------------------------------------------------------------
# Configuración
# ---------------------------------------------------------------------------
PUERTO_COM         = 'COM2'                    # Puerto del Arduino físico
BAUDIOS            = 9600
ARCHIVO_SALIDA     = 'autotune_hielera.csv'
TIMEOUT_SILENCIO_S = 60   # Segundos sin datos antes de cerrar (ciclos térmicos lentos)

# ---------------------------------------------------------------------------
# Conexión
# ---------------------------------------------------------------------------
print(f"Conectando al puerto {PUERTO_COM}...")
try:
    ser = serial.Serial(PUERTO_COM, BAUDIOS, timeout=1)
    time.sleep(2)  # Esperar inicialización del Arduino
    print(f"Conectado. Guardando datos en '{ARCHIVO_SALIDA}'.")
    print("Presiona Ctrl+C para detener manualmente.\n")

    resultados_pid = []   # Acumula las líneas del bloque de resultados

    with open(ARCHIVO_SALIDA, mode='w', newline='') as archivo_csv:
        escritor = csv.writer(archivo_csv)
        escritor.writerow(['Tiempo_s', 'Temperatura_C', 'Rele_PWM'])
        archivo_csv.flush()

        ultimo_dato = time.time()

        while True:
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

            # --- Fin del experimento ---
            if linea.startswith('FIN,'):
                motivo = linea[4:]
                print(f"\nArduino: fin del experimento → {motivo}")
                break

            # --- Líneas de comentario del Arduino (# ...) ---
            if linea.startswith('#'):
                contenido = linea[1:].strip()

                # Progreso de ciclos: mostrar en consola en tiempo real
                if contenido.startswith('Ciclo'):
                    print(f"  ► {contenido}")

                # Bloque de resultados PID: acumular para mostrar al final
                elif any(k in contenido for k in ('Kp', 'Ki', 'Kd', 'Ku', 'Tu', 'RESULT', '====')):
                    resultados_pid.append(contenido)

                # Resto de mensajes informativos
                else:
                    print(f"[Arduino] {contenido}")
                continue

            # --- Ignorar cabecera reenviada por el Arduino ---
            if linea.startswith('Tiempo'):
                continue

            # --- Dato CSV normal ---
            datos = linea.split(',')
            if len(datos) == 3:
                escritor.writerow(datos)
                archivo_csv.flush()
                rele_on = int(datos[2]) > 0
                estado  = "ON " if rele_on else "OFF"
                print(f"t={datos[0]:>8} s | T={datos[1]:>6} °C | Relé {estado}")

except KeyboardInterrupt:
    print("\nCaptura detenida por el usuario.")
except Exception as e:
    print(f"Error: {e}")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Puerto serie cerrado.")

    print(f"\nDatos guardados en '{ARCHIVO_SALIDA}'.")

    if resultados_pid:
        print("\n" + "=" * 52)
        print("  RESULTADOS DEL AUTOTUNE PID")
        print("=" * 52)
        for linea in resultados_pid:
            if linea.strip('= '):
                print(f"  {linea}")
        print("=" * 52)
