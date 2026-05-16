from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


VREF = 5.0
ADC_COUNTS = 1023.0
SERIES_RESISTOR_OHMS = 10_000.0

NTC_R25_OHMS = 10_000.0
NTC_T25_KELVIN = 298.15
NTC_BETA_25_50 = 3380.0

STEINHART_A = 4.820105e-4
STEINHART_B = 3.209353e-4
STEINHART_C = -1.061439e-7

TEMP_MIN_C = -55.0
TEMP_MAX_C = 125.0
TARGET_TEMPERATURES_C = (-55.0, -25.0, 25.0, 75.0, 125.0)


def adc_to_voltage(adc_codes: np.ndarray) -> np.ndarray:
	return (adc_codes * VREF) / ADC_COUNTS


def divider_voltage_to_resistance(output_voltage: np.ndarray) -> np.ndarray:
	return SERIES_RESISTOR_OHMS * output_voltage / (VREF - output_voltage)


def resistance_to_celsius_beta(resistance_ohms: np.ndarray) -> np.ndarray:
	inverse_temperature = (1.0 / NTC_T25_KELVIN) + (
		np.log(resistance_ohms / NTC_R25_OHMS) / NTC_BETA_25_50
	)
	return (1.0 / inverse_temperature) - 273.15


def resistance_to_celsius_steinhart_hart(resistance_ohms: np.ndarray) -> np.ndarray:
	log_resistance = np.log(resistance_ohms)
	inverse_temperature = (
		STEINHART_A
		+ (STEINHART_B * log_resistance)
		+ (STEINHART_C * np.power(log_resistance, 3.0))
	)
	return (1.0 / inverse_temperature) - 273.15


def local_resolution_c_per_lsb(temperatures_c: np.ndarray) -> np.ndarray:
	resolution = np.empty_like(temperatures_c)
	resolution[1:-1] = np.abs(temperatures_c[2:] - temperatures_c[:-2]) / 2.0
	resolution[0] = np.abs(temperatures_c[1] - temperatures_c[0])
	resolution[-1] = np.abs(temperatures_c[-1] - temperatures_c[-2])
	return resolution


def sort_and_crop(
	temperatures_c: np.ndarray, values: np.ndarray
) -> tuple[np.ndarray, np.ndarray]:
	mask = (temperatures_c >= TEMP_MIN_C) & (temperatures_c <= TEMP_MAX_C)
	temperatures_in_range = temperatures_c[mask]
	values_in_range = values[mask]
	order = np.argsort(temperatures_in_range)
	return temperatures_in_range[order], values_in_range[order]


def sample_curve(
	temperatures_c: np.ndarray, values: np.ndarray, targets_c: tuple[float, ...]
) -> list[tuple[float, float]]:
	sampled = np.interp(np.array(targets_c), temperatures_c, values)
	return list(zip(targets_c, sampled))


def annotate_samples(
	axis: plt.Axes,
	temperatures_c: np.ndarray,
	values: np.ndarray,
	targets_c: tuple[float, ...],
	color: str,
	label_prefix: str,
	y_offset_points: float,
) -> None:
	for target_c, sampled_value in sample_curve(temperatures_c, values, targets_c):
		axis.scatter(target_c, sampled_value, color=color, s=36, zorder=3)
		axis.annotate(
			f"{label_prefix}\n({target_c:.0f}, {sampled_value:.3f})",
			xy=(target_c, sampled_value),
			xytext=(6, y_offset_points),
			textcoords="offset points",
			fontsize=8,
			color=color,
			bbox={"boxstyle": "round,pad=0.2", "fc": "white", "ec": color, "alpha": 0.8},
		)


def build_curves() -> dict[str, np.ndarray]:
	adc_codes = np.arange(1.0, ADC_COUNTS)
	output_voltage = adc_to_voltage(adc_codes)
	resistance_ohms = divider_voltage_to_resistance(output_voltage)

	temperature_beta_c = resistance_to_celsius_beta(resistance_ohms)
	temperature_steinhart_c = resistance_to_celsius_steinhart_hart(resistance_ohms)

	resolution_beta_c_per_lsb = local_resolution_c_per_lsb(temperature_beta_c)
	resolution_steinhart_c_per_lsb = local_resolution_c_per_lsb(temperature_steinhart_c)

	temperature_beta_c, adc_beta = sort_and_crop(temperature_beta_c, adc_codes)
	resolution_beta_c_per_lsb = sort_and_crop(
		resistance_to_celsius_beta(resistance_ohms),
		local_resolution_c_per_lsb(resistance_to_celsius_beta(resistance_ohms)),
	)[1]

	temperature_steinhart_c, adc_steinhart = sort_and_crop(temperature_steinhart_c, adc_codes)
	resolution_steinhart_c_per_lsb = sort_and_crop(
		resistance_to_celsius_steinhart_hart(resistance_ohms),
		local_resolution_c_per_lsb(resistance_to_celsius_steinhart_hart(resistance_ohms)),
	)[1]

	return {
		"temperature_beta_c": temperature_beta_c,
		"temperature_steinhart_c": temperature_steinhart_c,
		"adc_beta": adc_beta,
		"adc_steinhart": adc_steinhart,
		"resolution_beta_c_per_lsb": resolution_beta_c_per_lsb,
		"resolution_steinhart_c_per_lsb": resolution_steinhart_c_per_lsb,
	}


def save_temperature_vs_adc_plot(curves: dict[str, np.ndarray], output_dir: Path) -> Path:
	figure, axis = plt.subplots(figsize=(10, 6))
	axis.plot(
		curves["temperature_beta_c"],
		curves["adc_beta"],
		label="Beta",
		linewidth=2.0,
	)
	axis.plot(
		curves["temperature_steinhart_c"],
		curves["adc_steinhart"],
		label="Steinhart-Hart",
		linewidth=2.0,
		linestyle="--",
	)
	axis.set_title("NTC 10k: temperatura estimada vs codigo ADC")
	axis.set_xlabel("Temperatura estimada [C]")
	axis.set_ylabel("Codigo ADC [cuentas]")
	axis.grid(True, alpha=0.3)
	axis.legend()

	output_path = output_dir / "ntc_10k_temperatura_vs_adc.png"
	figure.tight_layout()
	figure.savefig(output_path, dpi=180)
	plt.close(figure)
	return output_path


def save_resolution_plot(curves: dict[str, np.ndarray], output_dir: Path) -> Path:
	figure, axis = plt.subplots(figsize=(10, 6))
	axis.plot(
		curves["temperature_beta_c"],
		curves["resolution_beta_c_per_lsb"],
		label="Resolucion con Beta",
		linewidth=2.0,
		color="tab:blue",
	)
	axis.plot(
		curves["temperature_steinhart_c"],
		curves["resolution_steinhart_c_per_lsb"],
		label="Resolucion con Steinhart-Hart",
		linewidth=2.0,
		linestyle="--",
		color="tab:orange",
	)
	annotate_samples(
		axis,
		curves["temperature_beta_c"],
		curves["resolution_beta_c_per_lsb"],
		TARGET_TEMPERATURES_C,
		"tab:blue",
		"Beta",
		10.0,
	)
	annotate_samples(
		axis,
		curves["temperature_steinhart_c"],
		curves["resolution_steinhart_c_per_lsb"],
		TARGET_TEMPERATURES_C,
		"tab:orange",
		"S-H",
		-24.0,
	)
	axis.set_title("NTC 10k: resolucion termica con ADC de 10 bits")
	axis.set_xlabel("Temperatura estimada [C]")
	axis.set_ylabel("Resolucion [C/LSB]")
	axis.set_xticks(np.arange(-50, 126, 25))
	axis.grid(True, alpha=0.3)
	axis.legend()

	output_path = output_dir / "ntc_10k_resolucion_beta_vs_steinhart.png"
	figure.tight_layout()
	figure.savefig(output_path, dpi=180)
	plt.close(figure)
	return output_path


def save_resolution_csv(curves: dict[str, np.ndarray], output_dir: Path) -> Path:
	row_count = min(len(curves["temperature_beta_c"]), len(curves["temperature_steinhart_c"]))
	csv_data = np.column_stack(
		[
			curves["temperature_beta_c"][:row_count],
			curves["resolution_beta_c_per_lsb"][:row_count],
			curves["temperature_steinhart_c"][:row_count],
			curves["resolution_steinhart_c_per_lsb"][:row_count],
		]
	)

	output_path = output_dir / "ntc_10k_resolucion_curvas.csv"
	np.savetxt(
		output_path,
		csv_data,
		delimiter=",",
		header=(
			"temperatura_beta_c,resolucion_beta_c_por_lsb,"
			"temperatura_steinhart_c,resolucion_steinhart_c_por_lsb"
		),
		comments="",
	)
	return output_path


def print_summary(curves: dict[str, np.ndarray]) -> None:
	beta_samples = sample_curve(
		curves["temperature_beta_c"],
		curves["resolution_beta_c_per_lsb"],
		TARGET_TEMPERATURES_C,
	)
	steinhart_samples = sample_curve(
		curves["temperature_steinhart_c"],
		curves["resolution_steinhart_c_per_lsb"],
		TARGET_TEMPERATURES_C,
	)

	print("Resolucion termica estimada para el NTC 10k con ADC de 10 bits")
	print("Divisor: 10 kOhm fijo + NTC a GND, Vref = 5 V")
	print()
	print("Modelo Beta")
	for temperature_c, resolution_c_per_lsb in beta_samples:
		print(f"  T = {temperature_c:6.1f} C -> {resolution_c_per_lsb:0.4f} C/LSB")
	print()
	print("Modelo Steinhart-Hart")
	for temperature_c, resolution_c_per_lsb in steinhart_samples:
		print(f"  T = {temperature_c:6.1f} C -> {resolution_c_per_lsb:0.4f} C/LSB")


def main() -> None:
	output_dir = Path(__file__).resolve().parent / "output"
	output_dir.mkdir(exist_ok=True)

	curves = build_curves()
	temperature_plot_path = save_temperature_vs_adc_plot(curves, output_dir)
	resolution_plot_path = save_resolution_plot(curves, output_dir)
	csv_path = save_resolution_csv(curves, output_dir)

	print_summary(curves)
	print()
	print(f"Grafica temperatura-ADC: {temperature_plot_path}")
	print(f"Grafica de resolucion: {resolution_plot_path}")
	print(f"Datos CSV: {csv_path}")


if __name__ == "__main__":
	main()
