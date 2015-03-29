#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
#include "local1.h"
#include <stdio.h> //printf
#include <time.h>
#include <ctime>

using namespace std;

int main(){

	//Medidor
	clock_t t_ini, t_fin;
	float secs;
	int instancias = 0;
	int repeticiones = 0;
	float acum_tiempo = 0;
	float acum_peso = 0;
	cin >> instancias >> repeticiones;

	for(int z = 0; z < instancias; z++){
		int n, m, k, u, v;
		float peso;
		Solucion sol;
		cin >> n;
		cin >> m;
		cin >> k;

		Grafo g(n, vector<float>(n, 0));

		for(int i = 0; i < m; i++){
			cin >> u;
			cin >> v;
			cin >> peso;
			u--;
			v--;
			g[u][v] = peso;
			g[v][u] = peso;
		}

		float mejor_tiempo = -1;
		float mejor_peso = -1;
		for(int i = 0; i < repeticiones; i++){
			Greedy greedy;
			Solucion solucion_inicial = greedy.resolver(g, k, n);

			t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo

			BusquedaLocal1 busquedaLocal(solucion_inicial);
			Solucion mejor_solucion= busquedaLocal.resolver(g, k, n);

			/*BusquedaLocal2 busquedaLocal2(solucion_inicial);
			Solucion mejor_solucion= busquedaLocal2.resolver(g, k, n);
			busquedaLocal2.imprimir_solucion(true, k, g);*/
			peso = busquedaLocal.getPeso();
			t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.

			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".ss
			if(secs < mejor_tiempo || mejor_tiempo == -1) mejor_tiempo = secs;
			if(peso < mejor_peso || mejor_peso == -1) mejor_peso = peso;
		}
		printf("Mejor tiempo: %f, Mejor peso: %f\n", mejor_tiempo, mejor_peso);
		acum_tiempo += mejor_tiempo;
		acum_peso += mejor_peso;

	}
	printf("Tiempo promedio: %f, Peso promedio: %f\n", acum_tiempo/instancias, acum_peso/instancias);

	return 0;
}
