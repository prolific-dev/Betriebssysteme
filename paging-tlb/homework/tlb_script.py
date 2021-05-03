from numpy import *
import matplotlib.pyplot as plt
import subprocess

if __name__ == '__main__':
    print("Anzahl an Veruchen: ")
    nr_trials = int(input())
    max_pages = 4096
    print("Starte mit {} Versuchen... (Bitte warten)".format(nr_trials))
    subprocess.run(["gcc", "-O0", "-o", "tlb.out", "tlb.c", "-Wall"])

    x = [2**z for z in range(1, 14)]
    y = []
    for i in x:
        process = subprocess.run(['./tlb.out', '-p {}.'.format(i), '-t {}'.format(nr_trials)], stdout=subprocess.PIPE,
                                 text=True)
        result = process.stdout
        y.append(float(result))

    fig = plt.figure()
    plt.plot(x, y, marker='o', linestyle='--', color='blue')
    plt.margins(0)
    plt.xlabel('Frame-Anzahl')
    plt.xscale('log')
    plt.ylabel('Zeit pro Zugriff (ns)')
    plt.title('TLB Zeitmessung')
    plt.savefig('tlb.png', dpi=227)
    plt.show()

    print("tlb.png wurde erfolgreich erzeugt")
