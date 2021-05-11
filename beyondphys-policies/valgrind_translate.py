with open('valgrind_out.txt') as f:
    lines = f.readlines()
    
with open('vpn-reference.txt', 'w') as v:
    
    for line in lines:
        tmp = ''
        if line.startswith('I'):
            tmp = '0x' + line.split('  ')[1].split(',')[0]
            tmpHex = int(tmp, 16)
            tmpHex = (tmpHex & 0xfffff000) >> 12
            tmp = str(tmpHex)
        elif line.startswith(' S') or line.startswith(' L') or line.startswith(' M'):
            tmp = '0x' + line.split(' ')[2].split(',')[0]
            tmpHex = int(tmp, 16)
            tmpHex = (tmpHex & 0xfffff000) >> 12
            tmp = str(tmpHex)
            
        v.write(tmp + '\n')
    

f.close()
v.close()
      
