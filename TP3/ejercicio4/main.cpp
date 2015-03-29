#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
#include "local1.h"
#include "local2.h"

using namespace std;

int main(){

	/*
	conjunto c;

	NodoConPeso n1(1,0.1,1);
	NodoConPeso n2(2,0.5,1);
	c.insert(n1);

	cout << c.count(n1) << endl;
	cout << c.count(n2) << endl;
	*/

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


	Greedy greedy;

	Solucion solucion_inicial = greedy.resolver(g, k, n);
	greedy.imprimir_solucion(true);

/*
	BusquedaLocal1 busquedaLocal(solucion_inicial);

    Solucion mejor_solucion= busquedaLocal.resolver(g, k, n);
	busquedaLocal.imprimir_solucion(true, k, g);
*/
	BusquedaLocal2 busquedaLocal2(solucion_inicial);
	Solucion mejor_solucion= busquedaLocal2.resolver(g, k, n);
	busquedaLocal2.imprimir_solucion(true, k, g);

	return 0;
}
