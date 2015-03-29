#ifndef __GREEDY__H__
#define __GREEDY__H__

#include <vector>

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;
typedef set<int> Conjunto;
typedef vector<Conjunto> Particion;

class Greedy {

public:
	Greedy(int alpha, int beta);
	~Greedy();
	Solucion resolver(const Grafo& g, int cant_particiones, int cant_nodos);
	void imprimir_solucion(bool imprimir_peso) const;
	int getPeso();

private:
	Solucion solucion;
	int alpha;
	int beta;
	int peso_de_solucion;
};

#endif
