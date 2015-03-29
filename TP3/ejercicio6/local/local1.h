#ifndef __LOCAL1__H__
#define __LOCAL1__H__

#include <vector>

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;

class BusquedaLocal1{
public:
	BusquedaLocal1(const Solucion& sol);
	~BusquedaLocal1();
	Solucion resolver(const Grafo& g, int cant_partciones, int cant_nodos);
	void imprimir_solucion(bool imprimir_solucion, int cant_particiones, const Grafo& g) const;
	float getPeso();

private:
	Solucion solucion_inicial;
	Solucion mejor_vecino;
	float peso_mejor_vecino;
};

#endif
