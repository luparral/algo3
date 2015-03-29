#include <utility>
#include <vector>
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stdio.h> //printf
#include <set>
#include <time.h>
#include "greedy.h"
using namespace std;

//Typedefs
typedef float				 Peso;
typedef int 				 Nodo;
typedef vector<Nodo>		 Nodos;
typedef vector<int> 		 Sol;
typedef vector<Nodo>		 Conj;
typedef vector<Conj > 	 		Part;
typedef vector<vector<Peso> > Pesos;



//Prototipado de funciones
int pesoDeConjunto(Conj& conjunto, Pesos& pesos);
int resolver(int n, Pesos& pesos, int k, Sol& solucion, Grafo& grafo);
void backtracking(Nodos& nodos, Pesos& pesos, Part& particion, int& optima_cant, Part& resultado, int k);
int pesoDeParticion(Part& particion, Pesos& pesos);
int buscarNodo(int prod, Part& resultado);
int pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos);
Part generarParticionInicial(int n, Pesos& pesos, int k, Grafo& grafo);
void getPartitions(Pesos& pesos, int optimo_peso, int n, int k, vector<Part>& itetarivePartitions);
void print(Part& particion);
void printPartitions(vector<Part>& myParticions,Pesos& pesos);
Part convertir_solucion_en_particion(const Sol& sol, int cant_particiones);

int main(){

	double secs;
	clock_t t_ini, t_fin;
	int instancias = 0;
	int repeticiones = 0;
	double acum_tiempo = 0;
	int acum_peso = 0;
	cin >> instancias >> repeticiones;

	for(int z = 0; z < instancias; z++){
		int n, m, k;
		cin >> n >> m >> k;

		//Almaceno los pesos de cada par de nodos (pesos de aristas)
		Pesos pesos_ij;
		pesos_ij.reserve(n);
		for(int i = 1; i < n; i++){
			vector<Peso> pesos_de_arista_i(n-i, 0);
			pesos_ij.push_back(pesos_de_arista_i);
		}

		Grafo g(n, vector<float>(n, 0));

		for (int i = 0; i < m; i++){
			int x, y, p;
			cin >> x >> y >> p;

			if(x < y)
				pesos_ij[x-1][y-x-1] = p;
			else
				pesos_ij[y-1][x-y-1] = p;

			x--;
			y--;
			g[x][y] = p;
			g[y][x] = p;

		}

		int peso = 0;
		int mejor_peso = -1;
		double mejor_tiempo = -1;
		for(int i = 0; i < repeticiones; i++){

			t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo

			//Con los datos almacenados en estructuras, llamo a la resolucion del problema
			Sol solucion;
			solucion.reserve(n);
			if (k < n)
				peso = resolver(n, pesos_ij, k, solucion, g);
			else
				for(int i = 1; i <= n; i++)	solucion.push_back(i);

			t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.


	    	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".
	    	if(secs < mejor_tiempo || mejor_tiempo == -1) mejor_tiempo = (double)secs;
			if(peso < mejor_peso || mejor_peso == -1) mejor_peso = peso;
		}
		printf("Mejor tiempo: %f, Mejor peso: %d\n", mejor_tiempo, mejor_peso);
		acum_tiempo += mejor_tiempo;
		acum_peso += mejor_peso;
	}
	printf("Tiempo promedio: %f, Peso promedio: %d\n", acum_tiempo/instancias, acum_peso/instancias);

	return 0;
}

int resolver(int n, Pesos& pesos, int k, Sol& solucion, Grafo& grafo){
	Part resultado;
	vector<Part> itetarivePartitions(1, Part(1,Conj(1,1)));

	resultado = generarParticionInicial(n,pesos,k,grafo);
	int optimo_peso = pesoDeParticion(resultado,pesos);

	getPartitions(pesos, optimo_peso, n, k, itetarivePartitions);

	for (int i = 0; i < itetarivePartitions.size(); ++i){
		if(pesoDeParticion(itetarivePartitions[i],pesos) < optimo_peso){
			resultado = itetarivePartitions[i];
			optimo_peso = pesoDeParticion(itetarivePartitions[i],pesos);
		}
	}
	int pesoDelResultado = pesoDeParticion(resultado,pesos);

	//Armo y devuelvo la solucion
	for(int i = 1; i <= n; i++)
		solucion.push_back(buscarNodo(i,resultado));

	return pesoDelResultado;
}

void getPartitions(Pesos& pesos, int optimo_peso, int n, int k, vector<Part>& itetarivePartitions){
	for (int i = 2; i <= n; ++i){
		//cout << i << " " << itetarivePartitions.size() << endl;
		int itetarivePartitionsCant = itetarivePartitions.size();
		for (int j = 0; j < itetarivePartitionsCant; ++j){
			Part actualPart = itetarivePartitions[j];

			if(itetarivePartitions[j].size() < k)
				itetarivePartitions[j].push_back(Conj(1,i));
			else{
				itetarivePartitions.erase(itetarivePartitions.begin()+j);
				j--;
				itetarivePartitionsCant--;
			}

			for (int a = 0; a < actualPart.size(); ++a){
				actualPart[a].push_back(i);
				if(pesoDeParticion(actualPart,pesos) < optimo_peso){
					itetarivePartitions.push_back(actualPart);

					if(i == n)
						optimo_peso = pesoDeParticion(actualPart,pesos);

				}
				actualPart[a].pop_back();
			}
		}

	}

	return;
}


int pesoDeParticion(Part& particion, Pesos& pesos){
	int acum = 0;
	for (int i = 0; i < particion.size(); i++)
		acum += pesoDeConjunto(particion[i],pesos);

	return acum;
}

int pesoDeConjunto(Conj& conjDeNodos, Pesos& pesos){
	if(conjDeNodos.size() < 2)
		return 0;

	int acum = 0;
	for(int i = 0; i < conjDeNodos.size()-1; i++)
		for(int j = i+1; j < conjDeNodos.size(); j++)
			acum += pesoDeArista(conjDeNodos[i], conjDeNodos[j], pesos);

	return acum;
}

int pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos){
	nodo_i--;
	nodo_j--;

	if(nodo_i < nodo_j)
		return pesos[nodo_i][nodo_j-nodo_i-1];
	else
		return pesos[nodo_j][nodo_i-nodo_j-1];
}

void print(Part& particion){
	for(int i = 0; i < particion.size(); i++){
		cout << "[ ";
		for(int j = 0; j < particion[i].size(); j++){
			cout << particion[i][j] << " " ;
		}
		cout << "] ";
	}
	// cout << endl;
	return;
}

int buscarNodo(int nodo, Part& resultado){
	for(int i = 0; i < resultado.size(); i++)
		for(int j = 0; j < resultado[i].size(); j++)
	    	if(nodo == resultado[i][j])
				return i+1;
	return 0;
}

void printPartitions(vector<Part>& myParticions, Pesos& pesos){
	for (int i = 0; i < myParticions.size(); ++i){
		print(myParticions[i]);
		cout << "Peso: " << pesoDeParticion(myParticions[i],pesos);
		cout << endl;
	}
}

Part generarParticionInicial(int n, Pesos& pesos, int k, Grafo& grafo){
	Greedy greedy;
	Solucion solucion_inicial = greedy.resolver(grafo, k, n);

	int cantConjs = 0;
	for (int i = 0; i < n; ++i)
		if(cantConjs < solucion_inicial[i])
			cantConjs = solucion_inicial[i];

	return convertir_solucion_en_particion(solucion_inicial, cantConjs);
}

Part convertir_solucion_en_particion(const Sol& sol, int cant_particiones){
	Part particion(cant_particiones, Conj());
	for(int i = 0; i < sol.size(); i++){
		particion[sol[i]-1].push_back(i+1);
	}

	return particion;
}
