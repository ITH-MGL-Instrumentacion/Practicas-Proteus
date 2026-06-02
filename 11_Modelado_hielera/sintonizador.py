import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# 1. Cargar los datos experimentales obtenidos
# Asegúrate de subir el archivo 'datos_hielera.csv' a tu entorno de Colab
df = pd.read_csv('datos_hielera.csv', names=['Tiempo', 'Temperatura', 'PWM'])

# Filtrar los datos a partir de que se aplica el escalón (t >= 5s)
df_escalon = df[df['Tiempo'] >= 5.0].copy()
df_escalon['Tiempo_Relativo'] = df_escalon['Tiempo'] - 5.0 # T0 del experimento

t_exp = df_escalon['Tiempo_Relativo'].values
y_exp = df_escalon['Temperatura'].values

# Parámetros del experimento conocidos
T_amb = y_exp[0]    # Temperatura inicial de la hielera antes del escalón
U_step = 255        # Amplitud del escalón de entrada (PWM máximo)

# 2. Definición matemática de la respuesta al escalón de 2do orden sobreamortiguado
def respuesta_segundo_orden(t, K, tau1, tau2):
    # Evitar divisiones por cero en el algoritmo de ajuste numérico
    if np.abs(tau1 - tau2) < 1e-4:
        tau2 += 1e-3
    
    # Ecuación analítica en el dominio del tiempo
    y = T_amb + K * U_step * (1.0 - (tau1 * np.exp(-t / tau1) - tau2 * np.exp(-t / tau2)) / (tau1 - tau2))
    return y

# 3. Ajuste de curvas (System Identification) utilizando Scipy
# Estimaciones iniciales: K_guess = (T_max - T_amb)/255, tau1_guess, tau2_guess
T_max_estimado = y_exp[-1]
K_guess = (T_max_estimado - T_amb) / U_step

popt, _ = curve_fit(respuesta_segundo_orden, t_exp, y_exp, p0=[K_guess, 40.0, 15.0], bounds=(0, np.inf))
K_est, tau1_est, tau2_est = popt

print("="*50)
print("PARÁMETROS IDENTIFICADOS DEL SISTEMA:")
print(f"Ganancia del Sistema (K): {K_est:.5f} °C / unidad PWM")
print(f"Constante de tiempo de la hielera (tau_1): {tau1_est:.2f} segundos")
print(f"Constante de tiempo del sensor/foco (tau_2): {tau2_est:.2f} segundos")
print("="*50)

# 4. Cálculo de Parámetros PID mediante Reglas de Sintonía IMC (Internal Model Control)
# Seleccionamos una constante de tiempo de lazo cerrado deseada (lambda). 
# Una regla robusta es hacerla igual a la constante de tiempo mayor dividida entre 3.
lambda_cl = tau1_est / 3.0 

Kp_pid = (tau1_est + tau2_est) / (K_est * lambda_cl)
Ti_pid = tau1_est + tau2_est
Td_pid = (tau1_est * tau2_est) / (tau1_est + tau2_est)

Ki_pid = Kp_pid / Ti_pid
Kd_pid = Kp_pid * Td_pid

print("SINTONÍA DEL CONTROLADOR PID CALCULADA:")
print(f"Ganancia Proporcional (Kp): {Kp_pid:.4f}")
print(f"Ganancia Integral     (Ki): {Ki_pid:.4f}")
print(f"Ganancia Derivativa   (Kd): {Kd_pid:.4f}")
print("="*50)

# 5. Graficación de Resultados de la Identificación
plt.figure(figsize=(10, 5))
plt.plot(t_exp, y_exp, 'b.', label='Datos Experimentales (Proteus CSV)')
plt.plot(t_exp, respuesta_segundo_orden(t_exp, *popt), 'r-', linewidth=2.5, label='Modelo Identificado de 2do Orden')
plt.title('Identificación de la Dinámica Térmica de la Hielera')
plt.xlabel('Tiempo desde el Escalón (segundos)')
plt.ylabel('Temperatura (°C)')
plt.grid(True)
plt.legend()
plt.show()