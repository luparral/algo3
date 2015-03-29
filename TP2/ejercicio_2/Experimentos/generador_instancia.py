import random



print "Insert amount of instances: "

instances = input();
instances = int(instances);

print "Insert size of matrix: "

size = input()
size = int(size)


print "Insert amount of knights: "
knights = input()
knights = int(knights)

f = open(str(instances)+'instancias_n'+str(size)+'_k'+str(knights)+'.txt', 'w')
f.write(str(instances)+'\n')

for k in range(0, instances):

	f.write(str(size)+' '+str(knights)+'\n')

	for i in range(0, knights):
		x = random.randint(1, size)
		y = random.randint(1, size)
		f.write(str(x)+' '+str(y)+'\n')

	f.write('\n')

f.write('0')
f.close()