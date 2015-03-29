#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "greedy.h"
#include <limits> 

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;
typedef set<int> Conjunto;
typedef vector<Conjunto> Particion;


int peso_intraparticion(int i, const Grafo& g, const Conjunto& conjunto);
int peso_de_particion(const Particion& particion, int k, const Grafo& g);

struct NodoConPeso {
	int nodo;
	float peso;

	NodoConPeso(int n, float w):nodo(n),peso(w){}
};

struct ComparacionPorPeso {
    inline bool operator() (const NodoConPeso& n1, const NodoConPeso& n2) {
        return (n1.peso > n2.peso);
    }
};

Greedy::Greedy(){}

Greedy::~Greedy(){}

Solucion Greedy::resolver(const Grafo& g, int cant_particiones, int cant_nodos){
	
	Particion particiones(cant_particiones, Conjunto());
	Conjunto conjuntoNodos;
	for(int nodo = 0; nodo < cant_nodos; nodo++){
		conjuntoNodos.insert(nodo);
	}

	vector<NodoConPeso> pesos;
	for(int nodo = 0; nodo < cant_nodos; nodo++) {
		pesos.push_back(NodoConPeso(nodo, peso_intraparticion(nodo, g, conjuntoNodos)));
	}

	sort(pesos.begin(), pesos.end(), ComparacionPorPeso());

	Solucion posicionDelNodo(cant_nodos,0);

	for(int nodo = 0; nodo < pesos.size(); nodo++){
		//cambio a partir de la correccion de Diego: para cada nodo sacado en orden segun su peso, su menor peso inicial es infinito.
		float menorPeso = std::numeric_limits<float>::max();
		int indicePariticion = 0;

		for(int l = 0; l < cant_particiones; l++){
			float pesoEnL = peso_intraparticion(pesos[nodo].nodo, g, particiones[l]);
			if( pesoEnL <= menorPeso){
				indicePariticion = l;
				menorPeso = pesoEnL;
			}
		}

		conjuntoNodos.erase(pesos[nodo].nodo);
		particiones[indicePariticion].insert(pesos[nodo].nodo);
		posicionDelNodo[pesos[nodo].nodo]=indicePariticion+1;

	}

	this->solucion = posicionDelNodo;
	this->peso_de_solucion = peso_de_particion(particiones, cant_particiones, g);
	return posicionDelNodo;
}

void Greedy::imprimir_solucion(bool imprimir_peso) const {
	if(imprimir_peso){
		cout << this->peso_de_solucion << endl;
	}

	for(int i = 0; i < solucion.size()-1; i++){
		cout << solucion[i] << " ";
	}
	cout << solucion[solucion.size()-1] << endl;
}

int peso_intraparticion(int i, const Grafo& g, const Conjunto& conjunto){
	if(conjunto.empty()){
		return 0;
	} else {
		int sumaPesos = 0;
		for(Conjunto::const_iterator it = conjunto.begin(); it != conjunto.end(); it++){
			sumaPesos += g[i][(*it)];
		}
		return sumaPesos;
	}
}

int peso_de_particion(const Particion& particion, int k, const Grafo& g){
	int sumaPesos = 0;
	for(int i = 0; i < particion.size(); i++){
		for(Conjunto::const_iterator it = particion[i].begin(); it != particion[i].end(); it++){
			sumaPesos += g[i][(*it)];
		}
	}
	return sumaPesos;
}

/**
int main (){
	int cant_nodos, cant_aristas, cant_particiones, nodo_u, nodo_v;
	float peso;

	cin >> cant_nodos;
	cin >> cant_aristas;
	cin >> cant_particiones;

	Grafo g(cant_nodos, vector<float>(cant_nodos, 0));
	
	for(int i = 0; i < cant_aristas; i++){
		cin >> nodo_u;
		cin >> nodo_v;
		cin >> peso;
		nodo_u--;
		nodo_v--;
		g[nodo_u][nodo_v] = peso;
		g[nodo_v][nodo_u] = peso;
	}

	Greedy greedy;

	greedy.resolver(g, cant_particiones, cant_nodos);

	greedy.imprimir_solucion();

	return 0;

}
**/