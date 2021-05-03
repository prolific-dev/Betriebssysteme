#! /usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

plt.xlabel('size of memory used by mem')
plt.ylabel('bandwith(mb/s)')

x = [4000, 4500, 5000, 5500, 6000, 6500]
y = [1722, 1738, 1658, 1705, 963, 711]

plt.plot(x, y, label='loop 0')
plt.savefig('plot.png', dpi=227)

x1 = [4000, 4500, 5000, 5500, 6000, 6500]
y1 = [7150, 7080, 6829, 7025, 83, 39]

plt.plot(x1, y1, label='loop 1-5')
plt.savefig('plot.png', dpi=227)

plt.legend()
plt.show()
