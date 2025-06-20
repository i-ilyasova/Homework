import numpy as np
import matplotlib.pyplot as plt

V0, V1 = 0, 5
h = 1e-6
t_pulse = 10e-6
t_delay = 1e-6
t_total = 320e-6
t = np.arange(0, t_total, h)
N = len(t)

def generate_signals(t, t_pulse):
    X1 = V0 + (V1 - V0) * ((t % (2*t_pulse)) < t_pulse)
    X2 = V0 + (V1 - V0) * ((t % (4*t_pulse)) < 2*t_pulse)
    X3 = V0 + (V1 - V0) * ((t % (8*t_pulse)) < 4*t_pulse)
    X4 = V0 + (V1 - V0) * ((t % (16*t_pulse)) < 8*t_pulse)
    a2 = V0 + (V1 - V0) * ((t % (32*t_pulse)) < 16*t_pulse)
    a1 = V0 + (V1 - V0) * ((t % (16*t_pulse)) < 8*t_pulse)
    return X1, X2, X3, X4, a1, a2

def logic_not(signal, delay_steps=0):
    result = V1 - signal
    if delay_steps > 0:
        result = np.roll(result, delay_steps)
        result[:delay_steps] = V0
    return result

def logic_and(sig1, sig2, delay_steps=0):
    result = np.where((sig1 > V1/2) & (sig2 > V1/2), V1, V0)
    if delay_steps > 0:
        result = np.roll(result, delay_steps)
        result[:delay_steps] = V0
    return result

def logic_or(signals, delay_steps=0):
    result = np.zeros_like(signals[0])
    for sig in signals:
        result = np.where((result > V1/2) | (sig > V1/2), V1, V0)
    if delay_steps > 0:
        result = np.roll(result, delay_steps)
        result[:delay_steps] = V0
    return result

def multiplexer_ideal(X1, X2, X3, X4, a1, a2):
    not_a1 = logic_not(a1)
    not_a2 = logic_not(a2)
    sel0 = logic_and(not_a1, not_a2)
    sel1 = logic_and(not_a1, a2)
    sel2 = logic_and(a1, not_a2)
    sel3 = logic_and(a1, a2)
    out0 = logic_and(X1, sel0)
    out1 = logic_and(X2, sel1)
    out2 = logic_and(X3, sel2)
    out3 = logic_and(X4, sel3)
    F = logic_or([out0, out1, out2, out3])
    return F

def multiplexer_with_delays(X1, X2, X3, X4, a1, a2):
    delay_steps = int(t_delay / h)
    not_a1 = logic_not(a1, delay_steps)
    not_a2 = logic_not(a2, delay_steps)
    sel0 = logic_and(not_a1, not_a2, delay_steps)
    sel1 = logic_and(not_a1, a2, delay_steps)
    sel2 = logic_and(a1, not_a2, delay_steps)
    sel3 = logic_and(a1, a2, delay_steps)
    out0 = logic_and(X1, sel0, delay_steps)
    out1 = logic_and(X2, sel1, delay_steps)
    out2 = logic_and(X3, sel2, delay_steps)
    out3 = logic_and(X4, sel3, delay_steps)
    F = logic_or([out0, out1, out2, out3], delay_steps)
    return F

X1, X2, X3, X4, a1, a2 = generate_signals(t, t_pulse)
F_ideal = multiplexer_ideal(X1, X2, X3, X4, a1, a2)
F_delayed = multiplexer_with_delays(X1, X2, X3, X4, a1, a2)

plt.figure(figsize=(15, 12))
plt.subplot(8, 1, 1)
plt.plot(t*1e6, X1)
plt.ylabel('X1')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 2)
plt.plot(t*1e6, X2)
plt.ylabel('X2')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 3)
plt.plot(t*1e6, X3)
plt.ylabel('X3')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 4)
plt.plot(t*1e6, X4)
plt.ylabel('X4')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 5)
plt.plot(t*1e6, a1)
plt.ylabel('a1')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 6)
plt.plot(t*1e6, a2)
plt.ylabel('a2')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 7)
plt.plot(t*1e6, F_ideal)
plt.ylabel('F\n(идеал)')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.subplot(8, 1, 8)
plt.plot(t*1e6, F_delayed)
plt.ylabel('F\n(задержки)')
plt.xlabel('Время, мкс')
plt.ylim(-0.5, 5.5)
plt.grid()
plt.tight_layout()
plt.show()
