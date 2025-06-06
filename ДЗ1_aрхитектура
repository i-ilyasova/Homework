import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

frequencies = [10e3, 5e3, 2e3]
amplitude = 5
duration = 1e-3
step = 1e-6
t = np.arange(0, duration, step)
signals = [amplitude * signal.sawtooth(2 * np.pi * f * t, 0.5) for f in frequencies]
forbidden_low = 2
forbidden_high = 4

def digital_output(signal, low, high, amplitude):
    output = np.zeros_like(signal)
    current_state = 0
    for i in range(len(signal)):
        if signal[i] > high:
            current_state = amplitude
        elif signal[i] < low:
            current_state = 0
        output[i] = current_state
    return output

digital_signals = [digital_output(sig, forbidden_low, forbidden_high, amplitude) for sig in signals]

fig, axs = plt.subplots(len(frequencies), 1, figsize=(10, 8), sharex=True)
for i, f in enumerate(frequencies):
    axs[i].plot(t * 1e3, signals[i], label=f'Треугольный сигнал {int(f/1e3)} кГц')
    axs[i].axhline(forbidden_low, color='red', linestyle='--', label='Нижняя граница')
    axs[i].axhline(forbidden_high, color='red', linestyle='--', label='Верхняя граница')
    axs[i].plot(t * 1e3, digital_signals[i], label='Цифровой выход')
    axs[i].set_ylabel('Амплитуда')
    axs[i].legend(loc='upper right')
    axs[i].grid(True)

axs[-1].set_xlabel('Время (мс)')
plt.suptitle('Треугольный сигнал и цифровой выход с запрещённой зоной')
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
