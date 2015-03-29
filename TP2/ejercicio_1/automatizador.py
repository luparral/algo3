#el archivo promediosEj1.txt debe ser vaciado cada vez que se ejecute este programa, ya que acumula resultados de iteraciones anteriores. TODO: hacer que al ingresar a este programa, ese archivo se borre.


import os

print "Cantidad de ciudades: "

cantCiudades = input()

print "Cantidad de vuelos: "

cantVuelos = input()

print "Cantidad de instancias: "

f = open('data.txt', 'w') #en data.txt se escriben los valores necesarios para pasarle ahora al generador de instancias y no tener que escribir por consola. No se si es necesario que este creado o si se crea en el open, chequear.

f.write(str(cantCiudades)+'\n'+str(cantVuelos))
f.close()



cantInstancias = input()

os.system("rm promediosEj1.txt")
os.system("g++ ejercicio1_medidordetiempos.cpp -o main")
os.system("g++ verificador.cpp -o verificador")

for x in range (0, int(cantInstancias)):	
	os.system("python generador_instancias_aleatorias.py data.txt")
	os.system("./verificador < instancia_aleatoria.txt")
	os.system("./main < instancia_aleatoria.txt")



promedios = []
archivo = open('promediosEj1.txt', 'r')
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
