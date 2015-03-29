#ifndef __LOCAL2__H__
#define __LOCAL2__H__

#include <vector>

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;

class BusquedaLocal2{
public:
	BusquedaLocal2(const Solucion& sol);
	~BusquedaLocal2();
	Solucion resolver(const Grafo& g, int cant_partciones, int cant_nodos);
	void imprimir_solucion(bool imprimir_solucion, int cant_particiones, const Grafo& g) const;
	float getPeso();

private:
	Solucion solucion_inicial;
	Solucion mejor_vecino;
	float peso_mejor_vecino;
};

#endif
