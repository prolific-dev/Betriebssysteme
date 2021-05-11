import subprocess
from random import randint


x = randint(5,15)

trace = []

for _ in range(x):
    trace.append(randint(0, x))

traceString = ','.join(str(i) for i in trace)


subprocess.call(['python3 paging-policy.py -p FIFO -c ' + traceString], shell=True)
subprocess.call(['python3 paging-policy.py -p LRU -c ' + traceString], shell=True)
subprocess.call(['python3 paging-policy.py -p OPT -c ' + traceString], shell=True)

