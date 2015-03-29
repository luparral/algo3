import random
import datetime
import time

ciudades = ['SanPe', 'Carac', 'Acapu', 'Calix', 'Macei', 'Distr', 'Forta', 'Guate', 'JoaoP', 'Barqu', 'Palmi','Natal', 'Salva', 'Vitor', 'SaoLu', 'Culia', 'Ciuda', 'Torre', 'Kings', 'CapeT', 'Chihu', 'Victo', 'Belem', 'Detro', 'Campin', 'NewOr', 'SanSa', 'Goian', 'Cuiab', 'Nuevo', 'Manau', 'Santa', 'Cucut', 'Perei', 'Medel', 'Balti', 'Juare', 'SanJu', 'Recif', 'Macap', 'Nelso', 'Marac', 'Cuern', 'BeloH', 'STLou', 'Araca', 'Tijua', 'Durba', 'Puert', 'Valen', 'Senda', 'Hiros', 'Saita', 'Kawas', 'Fukuo', 'Kioto', 'Kumag', 'Sappo', 'Nagoy', 'Osaka', 'Yokoh', 'Tokio', 'Chiyo', 'Shibu', 'Taito', 'Kodai', 'Higas', 'Kushi', 'Yamag', 'Nishi', 'Itami', 'Matsu', 'Hitac', 'Bunky', 'Suzuk', 'Araka', 'Odawa', 'Numaz', 'Totto', 'Kishi', 'Isesa']



def parAleatorioCiudades():
	ciudad_a = ciudades[random.randint(0,int(cantCiudades)-1)]
	ciudad_b = ciudades[random.randint(0,int(cantCiudades)-1)]

	while(ciudad_a == ciudad_b):
		ciudad_b = ciudades[random.randint(0,int(cantCiudades)-1)]

	return 	(ciudad_a, ciudad_b)

def parAleatorioHoras():
	ini = random.randint(0,100) #hasta la hora 99
	extra = random.randint(1,100)
	fin = ini + extra;
	
	return 	(ini,fin)


g = open('data.txt', 'r')
cantCiudades = g.readline()
int(cantCiudades)
n = g.readline()
int(n)

ciudad_a,ciudad_b= parAleatorioCiudades()

f = open('instancia_aleatoria.txt', 'w')
f.write(ciudad_a+' '+ciudad_b+' '+str(n)+'\n')

for i in range(0,int(n)):
	ciudad1,ciudad2 = parAleatorioCiudades()
	ini,fin = parAleatorioHoras()
	if(i != int(n)-1):
		f.write(ciudad1+' '+ciudad2+' '+str(ini)+' '+str(fin)+'\n')
	else:
		f.write(ciudad1+' '+ciudad2+' '+str(ini)+' '+str(fin))

f.close()

