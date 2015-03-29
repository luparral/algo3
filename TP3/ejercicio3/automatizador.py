import os
import sys

#print "Cantidad de nodos: "
n = 100
#print "Cantidad de aristas: "
m = 150
#print "Cantidad de particiones: "
k = 70
#print "Cantidad de instancias: "
cantInstancias = 40
f = open('data.txt', 'w')

f.write(str(n)+' '+str(m)+' '+str(k)+ ' ' + str(cantInstancias))

f.close()


os.system("rm promediosEj3.txt")
os.system("touch promediosEj3.txt")
os.system("g++ ejercicio3.v2_medidor.cpp -o main")
for x in range (0, int(cantInstancias)):
	os.system("g++ generador_input.cpp -o instancia")
	os.system("./instancia < data.txt")
	os.system("./main < random.txt")


promedios = []
archivo = open('promediosEj3.txt', 'r')
for line in archivo:
	promedios.append(float(line))

archivo.close()
acum = 0;
i = 0;
for prom in promedios:
	acum = acum+prom
	i = i+1

res = acum/i

print res