#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include <limits>
#include <stdio.h> //printf
#include <time.h>
#include <ctime>
#include "grasp.h"

using namespace std;


int main(){

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

	Grasp grasp(100, 25, 10);
	grasp.resolver(g, n, k);
	grasp.imprimir();

	return 0;
}
