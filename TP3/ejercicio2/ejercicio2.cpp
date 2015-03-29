#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
using namespace std;

//Typedefs
typedef float				 	Peso;
typedef int 				 	Nodo;
typedef vector<Nodo>		 	Nodos;
typedef vector<int> 		 	Solucion;
typedef vector<Nodo>		 	Conj;
typedef vector<Conj > 	 		Partition;
typedef vector<vector<Peso> > 	Pesos;



//Prototipado de funciones
Peso pesoDeParticion(Partition& particion, Pesos& pesos);
Peso pesoDeConjunto(Conj& conjunto, Pesos& pesos);
Peso pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos);
void resolver(int n, Pesos& pesos, int k, Solucion& solucion, Grafo& grafo);
int buscarNodo(int prod, Partition& resultado);
void getPartitions(Pesos& pesos, Peso optimo_peso, int n, int k, Partition& resultado);
Partition convertir_solucion_en_particion(const Solucion& sol, int cant_particiones);
Partition generarParticionInicial(int n, Pesos& pesos, int k, Grafo& grafo);

int main(){
	//Tomo input
	int n, m, k;
	cin >> n >> m >> k;

	//Almaceno los pesos de cada arista.
	Pesos pesos_ij;
	pesos_ij.reserve(n);
	for(int i = 1; i < n; i++){
		vector<Peso> pesos_de_arista_i(n-i, 0);
		pesos_ij.push_back(pesos_de_arista_i);
	}

	/*Grafo unicamente usado para pasar al algoritmo goloso como parámetro.
	 *No interviene en el algoritmo exacto en sí.*/
	Grafo g(n, vector<float>(n, 0));

	for (int i = 0; i < m; i++){
		int x, y;
		Peso p;
		cin >> x;
		cin >> y;
		cin >> p;
		
		if(x < y)
			pesos_ij[x-1][y-x-1] = p;
		else
			pesos_ij[y-1][x-y-1] = p;

		x--;
		y--;
		g[x][y] = p;
		g[y][x] = p;
	}

	//Con los datos almacenados en estructuras, llamo a la resolucion del problema
	Solucion solucion;
	solucion.reserve(n);
	if (k < n)
		resolver(n, pesos_ij, k, solucion, g);
	else
		for(int i = 1; i <= n; i++)	solucion.push_back(i);

	//Imprimo resultado en pantalla
	for(int i = 0; i < n; i++)	cout << solucion[i] << " ";
	cout << endl;
	return 0;
}

void resolver(int n, Pesos& pesos, int k, Solucion& solucion, Grafo& grafo){
	Partition resultado = generarParticionInicial(n, pesos, k, grafo);
	Peso optimoPesoParcial = pesoDeParticion(resultado, pesos);

	getPartitions(pesos, optimoPesoParcial, n, k, resultado);

	//Armo y devuelvo la solucion
	for(int i = 1; i <= n; i++)
		solucion.push_back(buscarNodo(i,resultado));

	return;
}

void getPartitions(Pesos& pesos, Peso optimo_peso, int n, int k, Partition& resultado){
	vector<Partition> itetarivePartitions(1, Partition(1,Conj(1,1)));

	for (int i = 2; i <= n; ++i){
		int itetarivePartitionsCant = itetarivePartitions.size();
		for (int j = 0; j < itetarivePartitionsCant; ++j){
			Partition actualPart = itetarivePartitions[j];

			if(itetarivePartitions[j].size() < k)
				itetarivePartitions[j].push_back(Conj(1,i));
			else{
				itetarivePartitions.erase(itetarivePartitions.begin()+j);
				j--;
				itetarivePartitionsCant--;
			}

			for (int a = 0; a < actualPart.size(); ++a){
				actualPart[a].push_back(i);
				Peso pesoParticion = pesoDeParticion(actualPart,pesos);
				if(pesoParticion < optimo_peso){
					itetarivePartitions.push_back(actualPart);
					
					if(i == n)
						optimo_peso = pesoParticion;
				
				}
				actualPart[a].pop_back();
			}
		}
	}

	if(itetarivePartitions.size() > 0)
		resultado = itetarivePartitions.back();

	return;
}


Peso pesoDeParticion(Partition& particion, Pesos& pesos){
	Peso acum = 0;
	for (int i = 0; i < particion.size(); i++)
		acum += pesoDeConjunto(particion[i],pesos);

	return acum;
}

Peso pesoDeConjunto(Conj& conjDeNodos, Pesos& pesos){
	if(conjDeNodos.size() < 2)
		return 0;

	Peso acum = 0;
	for(int i = 0; i < conjDeNodos.size()-1; i++)
		for(int j = i+1; j < conjDeNodos.size(); j++)
			acum += pesoDeArista(conjDeNodos[i], conjDeNodos[j], pesos);

	return acum;
}

Peso pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos){
	nodo_i--;
	nodo_j--;

	if(nodo_i < nodo_j)
		return pesos[nodo_i][nodo_j-nodo_i-1];
	else
		return pesos[nodo_j][nodo_i-nodo_j-1];
}

int buscarNodo(int nodo, Partition& resultado){
	for(int i = 0; i < resultado.size(); i++)
		for(int j = 0; j < resultado[i].size(); j++)
	    	if(nodo == resultado[i][j])
				return i+1;
	return 0;
}

Partition convertir_solucion_en_particion(const Solucion& sol, int cant_particiones){
	Partition particion(cant_particiones, Conj());
	for(int i = 0; i < sol.size(); i++){
		particion[sol[i]-1].push_back(i+1);
	}

	return particion;
}

Partition generarParticionInicial(int n, Pesos& pesos, int k, Grafo& grafo){
	Greedy greedy;
	Solucion solucion_inicial = greedy.resolver(grafo, k, n);
	
	int cantConjs = 0;
	for (int i = 0; i < n; ++i)
		if(cantConjs < solucion_inicial[i])
			cantConjs = solucion_inicial[i];

	return convertir_solucion_en_particion(solucion_inicial, cantConjs);
}