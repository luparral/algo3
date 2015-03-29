import random

f = open('instancia_aleatoria.txt', 'w')
print "estoy en generador de instancias"
n = 10
print n
m = 5
print m
k = 5
print k

f.write(str(n) + ' ' + str(m) + ' '+ str(k) + '\n')

matriz = []
for i in range(n):
    matriz.append([])
    for j in range(n):
        matriz[i].append(0)

#tengo que chequear no poner dos veces la misma arista.
for i in range(0,int(m)-1) :
	u = random.randint(0,n-1)
	r = range(1,u) + range(u+1, n-1)
	v = random.choice(r)	#random de los nodos menos u
	w = random.randint(0,100) #random de peso
	while matriz[u][v] != 1 | matriz[v][u] != 1:
		u = random.randint(0,n-1)
		r = range(1,u) + range(u+1, n-1)
		v = random.choice(r)	#random de los nodos menos u
	matriz[u][v] == 1
	matriz[v][u] == 1
	f.write(str(u) + ' ' + str(v) + ' ' + str(w) + '\n')

if(i != int(m)-1):
	f.write(str(u)+ ' '+ str(v)+ ' '+ str(w)+'\n')
else:
	f.write(str(u) + ' '+  str(v) + ' '+ str(w))

f.close()