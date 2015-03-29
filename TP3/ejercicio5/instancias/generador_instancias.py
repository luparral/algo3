import random

print "Instancias: "
instances = input()
instances = int(instances);

print "Repeticiones: "
repeticiones = input()
repeticiones = int(repeticiones)

#print "Alpha: "
#alpha = input()
#alpha = int(alpha)

#print "Beta: "
#beta = input()
#beta = int(beta)

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
        if graph[u-1][v-1] == -1:
            instance += str(u)+' '+str(v)+' '+str(p)+'\n'
            graph[u-1][v-1] = p
            graph[v-1][u-1] = p
            i += 1
    instance += '\n'

beta = 10
comando = ''

while beta <= 100:

    alpha = 10
    while alpha <= 100:

        filename = 'alpha'+str(alpha)+'_beta'+str(beta)+'_n'+str(n)+'_m'+str(m)+'_k'+str(k)+'.txt'
        f = open(filename, 'w')
        f.write(str(instances)+' '+str(repeticiones)+'\n')
        f.write(str(alpha)+' '+str(beta)+'\n')
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

        comando += './medidor < '+filename+' > alpha'+str(alpha)+'_beta'+str(beta)+'.txt && '
        alpha += 10

    beta += 10

f = open('comando.txt', 'w')
f.write(comando)
f.close
