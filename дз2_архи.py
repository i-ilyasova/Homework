import numpy as np
import matplotlib.pyplot as plt

inputs = np.array([[int(x) for x in f'{i:04b}'] for i in range(16)])

def priority_encoder(x1, x2, x3, x4):
    if x1 == 1:
        return 0, 0, 0
    elif x2 == 1:
        return 0, 1, 0
    elif x3 == 1:
        return 1, 0, 0
    elif x4 == 1:
        return 1, 1, 0
    else:
        return 0, 0, 1

outputs = np.array([priority_encoder(*row) for row in inputs])

fig, axs = plt.subplots(7, 1, figsize=(10, 12), sharex=True)

labels_in = ['X1', 'X2', 'X3', 'X4']
labels_out = ['a1', 'a2', 'EO']

for i in range(4):
    axs[i].step(range(16), inputs[:, i], where='post', label=labels_in[i])
    axs[i].set_ylim(-0.1, 1.1)
    axs[i].legend(loc='upper right')
    axs[i].grid(True)

for i in range(3):
    axs[i+4].step(range(16), outputs[:, i], where='post', label=labels_out[i])
    axs[i+4].set_ylim(-0.1, 1.1)
    axs[i+4].legend(loc='upper right')
    axs[i+4].grid(True)

axs[-1].set_xlabel('Комбинации входных сигналов (0-15)')
plt.suptitle('Входные и выходные сигналы приоритетного шифратора')
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()
