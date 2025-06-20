import numpy as np
import matplotlib.pyplot as plt


V0, V1 = 0.5, 4.5
h, t_total = 1e-6, 1e-3
t = np.arange(0, t_total, h)
freq = 5e3  


y = V0 + (V1 - V0) * ((t % (1/freq)) < (0.5/freq))

def rc_line(y, T):
    u = np.zeros_like(y)
    for i in range(1, len(y)):
        u[i] = u[i-1] + h * (y[i-1] - u[i-1]) / T
    return u

rc = rc_line(y, 1e-5)


noise = rc + np.random.uniform(-0.1, 0.1, size=rc.shape)


def logic_cascade(u, Umin, Umax):
    out = np.zeros_like(u)
    out[0] = V0 if u[0] < (Umin + Umax) / 2 else V1
    for i in range(1, len(u)):
        prev = out[i-1]
        if prev == V0:
            out[i] = V0 if u[i] < Umax else V1
        else:
            out[i] = V1 if u[i] > Umin else V0
    return out

out = logic_cascade(noise, 1.5, 3.5)


plt.figure(figsize=(10, 8))
plt.subplot(4,1,1)
plt.plot(t*1e3, y)
plt.title('Исходный меандр')
plt.grid()

plt.subplot(4,1,2)
plt.plot(t*1e3, rc)
plt.title('После RC-цепи')
plt.grid()

plt.subplot(4,1,3)
plt.plot(t*1e3, noise)
plt.title('С помехой')
plt.grid()

plt.subplot(4,1,4)
plt.plot(t*1e3, out)
plt.title('Выход логического каскада')
plt.grid()

plt.tight_layout()
plt.show()
