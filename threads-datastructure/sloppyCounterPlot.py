#! /usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
from subprocess import Popen, PIPE, STDOUT

plt.xlabel('Approximation Factor (S)')
plt.ylabel('Time (Seconds)')

trials = 10000
threads = 10
approxFact = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]


result = []


for af in approxFact:
    cmd = './sloppyCounter ' + str(trials) + ' ' + str(threads) + ' ' + str(af)
    p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE, stderr=STDOUT, close_fds=True)
    output = p.stdout.read()
    s = str(output)
    result.append(float(s.split()[4]))
    print(s.split()[4])

for x in result:
    x = x / 1000000000
    print(x)

result.sort()

plt.plot(approxFact, result, label='1')
plt.savefig('sloppyCounterPlot.png', dpi=227)

plt.legend()
plt.show()
