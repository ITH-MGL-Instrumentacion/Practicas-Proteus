import math

# Parametros nominales del NTC y del sistema de medicion.
# R0: resistencia nominal del NTC a la temperatura de referencia T0_C.
R0 = 10_000.0
# T0_C: temperatura de referencia del fabricante, normalmente 25 °C.
T0_C = 25.0
# BETA: constante beta del NTC para el modelo exponencial R(T).
BETA = 3380.0
# VEXC: voltaje de excitacion aplicado al puente de Wheatstone.
VEXC = 5.0

# Rango termico de diseno y punto de equilibrio deseado del puente.
# TMIN_C: limite inferior del rango de temperatura que se quiere medir con buena precision.
TMIN_C = 30.0
# TMAX_C: limite superior del rango de temperatura de diseno o proteccion.
TMAX_C = 50.0

# R_EQ: resistencia fija de referencia del puente. 
# Se elige para que el punto de equilibrio quede dentro del rango util del ADC.
R_EQ = 10e3

# Ventana util deseada a la salida del amplificador para usar la referencia interna de 1.1 V.
# VOUT_MIN: voltaje minimo objetivo a la salida del amplificador dentro del rango util.
VOUT_MIN = 0.10
# VOUT_MAX: voltaje maximo objetivo a la salida del amplificador dentro del rango util.
VOUT_MAX = 3.2


def ntc_resistance_beta(temp_c: float, r0: float = R0, beta: float = BETA, t0_c: float = T0_C) -> float:
    # Convierte temperatura a resistencia usando el modelo beta del NTC.
    t_k = temp_c + 273.15
    t0_k = t0_c + 273.15
    return r0 * math.exp(beta * (1.0 / t_k - 1.0 / t0_k))


def bridge_output(r_ntc: float, r_top: float, r_ref: float, v_exc: float = VEXC) -> float:
    # Calcula la salida diferencial del puente para una resistencia dada del NTC.
    s_plus = v_exc * r_ref / (r_top + r_ref)
    s_minus = v_exc * r_ntc / (r_top + r_ntc)
    return s_plus - s_minus


def solve_r_ntc_for_bridge(v_bridge: float, r_top: float, r_ref: float, v_exc: float = VEXC) -> float:
    # Despeja que resistencia del NTC produciria un Vbridge especifico.
    rhs = v_bridge / v_exc + r_ref / (r_top + r_ref)
    if rhs <= 0.0 or rhs >= 1.0:
        raise ValueError("No existe una resistencia positiva que cumpla ese Vbridge con estos parametros")
    return (rhs * r_top) / (1.0 - rhs)


def gain_from_two_points(vb_low: float, vb_high: float, vout_low: float = VOUT_MIN, vout_high: float = VOUT_MAX) -> float:
    # Obtiene la ganancia inicial para mapear dos puntos del puente a dos puntos de salida.
    return (vout_high - vout_low) / (vb_high - vb_low)


def vref_from_point(v_bridge: float, gain: float, vout_target: float) -> float:
    # Calcula la referencia de salida necesaria para que un punto caiga en el voltaje deseado.
    return vout_target - gain * v_bridge


def estimate_temp_from_resistance_beta(resistance: float, r0: float = R0, beta: float = BETA, t0_c: float = T0_C) -> float:
    # Convierte resistencia a temperatura usando la inversion del modelo beta.
    t0_k = t0_c + 273.15
    inv_t = (1.0 / t0_k) + (1.0 / beta) * math.log(resistance / r0)
    return (1.0 / inv_t) - 273.15


def main() -> None:
    # Resistencias del NTC en el limite inferior, superior y punto de equilibrio elegido.
    r_min = ntc_resistance_beta(TMIN_C)
    r_max = ntc_resistance_beta(TMAX_C)

    tbal_c = estimate_temp_from_resistance_beta(R_EQ)

    # Diseno simple del puente:
    # - ambas resistencias superiores toman el valor del NTC en la temperatura de equilibrio
    # - el brazo de referencia tambien se ajusta a ese mismo valor
    # Asi, cuando el NTC esta en tbal_c el puente queda balanceado y Vbridge = 0.
    r_top = R_EQ
    r_ref = R_EQ

    # Salida diferencial del puente en el limite inferior, el punto de equilibrio y el limite superior.
    vb_eq = bridge_output(R_EQ, r_top, r_ref)
    vb_min = bridge_output(r_min, r_top, r_ref)
    vb_max = bridge_output(r_max, r_top, r_ref)

    # A partir de esos extremos, se calcula una ganancia inicial y la referencia de salida.
    # La idea es mapear el rango termico de interes al rango util del ADC: 0.10 V a 1.00 V.
    gain = gain_from_two_points(vb_min, vb_max)
    vref = vref_from_point(vb_min, gain, VOUT_MIN)

    # Esta es la salida del amplificador justo en el equilibrio del puente.
    # No tiene por que valer 0 V: depende de la ganancia y del Vref elegidos.
    vout_eq = gain * vb_eq + vref

    print("=== NTC + Puente de Wheatstone + Amplificador de Instrumentacion ===")
    print(f"Rango de diseno: {TMIN_C:.1f} C a {TMAX_C:.1f} C")
    print(f"Temperatura de equilibrio propuesta: {tbal_c:.1f} C")
    print()
    print("Resistencias del NTC:")
    for temp in [TMIN_C, 35.0, tbal_c, 42.0, TMAX_C]:
        print(f"  T = {temp:5.1f} C  ->  R = {ntc_resistance_beta(temp):8.1f} ohm")
    print()
    print(f"Resistencia de equilibrio R_eq (Vbridge = 0): {R_EQ:.1f} ohm")
    print(f"Con ese puente: Vbridge(Tmin) = {vb_min:.6f} V")
    print(f"Con ese puente: Vbridge(Teq ) = {vb_eq:.6f} V")
    print(f"Con ese puente: Vbridge(Tmax) = {vb_max:.6f} V")
    print()
    # Voltajes de salida del amplificador con offset = 0 (solo ganancia aplicada).
    # Estos son los valores que se leen durante la calibracion antes de ajustar el trimmer de offset.
    vout_max_sin_offset = gain * vb_max   # con R(Tmax) conectada, offset = 0
    vout_min_sin_offset = gain * vb_min   # con R(Tmin) conectada, offset = 0
    span_calibracion = vout_max_sin_offset - vout_min_sin_offset
    print(f"Ganancia inicial sugerida: G = {gain:.4f}")
    print(f"Referencia de salida sugerida: Vref = {vref:.6f} V")
    print(f"Salida en equilibrio con ese Vref: Vout_eq = {vout_eq:.6f} V")
    print()
    print("Voltajes de calibracion (offset = 0, solo ganancia):")
    print(f"  Vout con R(Tmax) = {r_max:.1f} ohm (offset=0): {vout_max_sin_offset:+.4f} V")
    print(f"  Vout con R(Tmin) = {r_min:.1f} ohm (offset=0): {vout_min_sin_offset:+.4f} V")
    print(f"  Diferencia (span de calibracion): {span_calibracion:.4f} V")
    print()

    # Mostrar la diferencia entre cero del puente y cero de salida.
    # - Cero del puente: donde Vbridge = 0.
    # - Cero de salida: donde Vout = 0, lo cual depende tambien de Vref y de la ganancia.
    r_for_vout_zero = solve_r_ntc_for_bridge((0.0 - vref) / gain, r_top, r_ref)
    temp_for_vout_zero = estimate_temp_from_resistance_beta(r_for_vout_zero)
    print("Ceros del sistema:")
    print(f"  Cero del puente  (Vbridge = 0): R = {R_EQ:.1f} ohm  ->  T ~= {tbal_c:.2f} C")
    print(f"  Cero de salida   (Vout = 0 V):  R = {r_for_vout_zero:.1f} ohm  ->  T ~= {temp_for_vout_zero:.2f} C")
    print()
    print("Interpretacion:")
    print("  - Vbridge = 0 fija el equilibrio del puente.")
    print("  - Vout = 0 depende ademas de la referencia Vref y de la ganancia.")


if __name__ == "__main__":
    main()
