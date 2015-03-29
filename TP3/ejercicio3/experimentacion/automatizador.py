import os
import sys
from subprocess import Popen, PIPE 

ns = [550, 600, 650, 700, 750, 800, 850, 900, 950, 1000]
ks = [10]
cantInstancias = 10

os.system("g++ ejercicio3_indexacionCorrecta_medidor.cpp -o main")
os.system("g++ generadorInput.v2.cpp -o instancia")

prom_tiempos = open('prom_tiempos.txt', 'w')

for n in ns:
	m = n*4
	for k in ks:
		print("N:%d M:%d K:%d"%(n,m,k))
		f = open('data.txt', 'w')
		f.write(str(n)+' '+str(m)+' '+str(k)+ ' ' + str(cantInstancias))
		f.close()

		tiempos = []
		for x in range (0, int(cantInstancias)):
			print("\tInstancia %d"%x)			
			os.system("./instancia < data.txt")
			output = os.popen("./main < random.txt").read()
			tiempos.append(float(output.strip()))

		acum = 0;
		i = 0;
		for tiempo in tiempos:
			acum = acum+tiempo
			i = i+1

		print("\tN: %d PROMEDIO: %.2f"%(n, (acum/i)))
		prom_tiempos.write(str(n) + ',' + str(acum/i)+'\n')


prom_tiempos.close()	