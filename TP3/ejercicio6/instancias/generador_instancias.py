import random

print "Instancias: "
instances = input()
instances = int(instances);

print "Repeticiones: "
repeticiones = input()
repeticiones = int(repeticiones)


print "Cantidad de nodos: "
n = input()
n = int(n)

#print "Cantidad de aristas: (max "+str(n*(n-1)/2)+")"
#m = input()
#m = int(m)

m = n*(n-1)/2

print "Cantidad particiones: "
k = input()
k = int(k)

#Generador de mismas instancias para todos
instance = ''
for r in range(0, instances):
    graph = [[-1 for x in range(n)] for y in range(n)]

    instance += str(n)+' '+str(m)+' '+str(k)+'\n'
    i = 0
    while i < m:
        u = random.randint(1, n)
        v = random.randint(1, n)
        p = random.randint(0, 100)
        if graph[u-1][v-1] == -1 and u != v:
            instance += str(u)+' '+str(v)+' '+str(p)+'\n'
            graph[u-1][v-1] = p
            graph[v-1][u-1] = p
            i += 1
    instance += '\n'

beta = 10
comando = ''


filename = 'n'+str(n)+'_m'+str(m)+'_k'+str(k)+'.txt'
f = open(filename, 'w')
f.write(str(instances)+' '+str(repeticiones)+'\n')
f.write(instance)

#Generador de instancias diferentes para todos
#for r in range(0, instances):

#    f.write(str(n)+' '+str(m)+' '+str(k)+'\n')
#    for i in range(0, m):
#        u = random.randint(1, n)
#        v = random.randint(1, n)
#        p = random.randint(0, 100)
#        f.write(str(u)+' '+str(v)+' '+str(p)+'\n')
#
#    f.write('\n')

f.close()

comando += './medidor < ../instancias'+filename+' > experimentos/n'+str(n)+'_m'+str(m)+'_k'+str(k)+'out.txt && '

f = open('comando.txt', 'w')
f.write(comando)
f.close
