import numpy as np
import subprocess
import matplotlib.pyplot as plt

with open('valgrind_out.txt') as f:
    lines = f.readlines()
    
with open('vpn-reference.txt', 'w') as v:
    
    for line in lines:
        tmp = ''
        if line.startswith('I'):
            tmp = '0x' + line.split('  ')[1].split(',')[0]
            tmpHex = int(tmp, 16)
            hexValue = hex(tmpHex)
            hexValue = (hexValue & 0xfffff000) >> 12
            tmpHex = int(hexValue, 16)
            tmp = str(tmpHex)
        elif line.startswith(' S') or line.startswith(' L') or line.startswith(' M'):
            tmp = '0x' + line.split(' ')[2].split(',')[0]
            tmpHex = int(tmp, 16)
            hexValue = hex(tmpHex)
            hexValue = (hexValue & 0xfffff000) >> 12
            tmpHex = int(hexValue, 16)
            tmp = str(tempHex)
            
        v.write(tmp + '\n')
    

policies = ['FIFO', 'LRU', 'MRU', 'RAND']
hitrates_fifo = np.empty(60)
hitrages_lru = np.empty(60)
hitrates_mru = np.empty(60)
hitrates_rand = np.empty(60)

for cache in range(0, 60, 1):
    for policy in policies:
    
        policyCmd = 'python3 paging-policy.py -f ' + v.name + ' -C' + str(cache+1) + ' -p' + policy + ' -c -N'
        
        process = subprocess.Popen(policyCmd.split(), stdout=subprocess.PIPE)
        
        output, error = process.communicate()
        
        currentHitRate = 0
        
        currentHitRate = float(output.split('hitrate ')[1])
        
        if policy == 'FIFO':
            hitrates_fifo[cache] = currentHitRate
        if policy == 'LRU':
            hitrates_lru[cache] = currentHitRate
        if policy == 'MRU':
            hitrates_mru[cache] = currentHitRate
        if policy == 'RAND':
            hitrates_rand[cache] = currentHitRate
        
xaxis = np.arange(1, 61)
plt.xlabel('Cache Size (Blocks)')
plt.ylabel('Hit Rate (in %)')
plt.plot(xaxis, hitrate_fifo, label='FIFO')
plt.plot(xaxis, hitrate_lru, label='LRU')
plt.plot(xaxis, hitrate_mru, label='MRU')
plt.plot(xaxis, hitrate_rand, label='RAND')
plt.legend(loc='lower right')
plt.savefig('plot.png', dpi=300)
plt.show()

f.close()
v.close()
      
