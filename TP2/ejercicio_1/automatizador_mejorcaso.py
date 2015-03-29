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
	os.system("python generador_instancia_mejor_caso.py data.txt")
	os.system("./verificador < instancia_mejorCaso.txt")
	os.system("./main < instancia_mejorCaso.txt")
