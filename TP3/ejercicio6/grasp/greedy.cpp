#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>
#include <limits>
#include <time.h>
#include "greedy.h"

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;
typedef set<int> Conjunto;
typedef vector<Conjunto> Particion;

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

struct ComparacionPesoTupla {
	inline bool operator() (const pair<NodoConPeso, int>& t1, const pair<NodoConPeso, int>& t2){
		return (t1.first.peso > t2.first.peso);
	}
};

int peso_intraparticion(int i, const Grafo& g, const Conjunto& conjunto);
int peso_de_particion(const Particion& particion, int k, const Grafo& g);
int pickOne(vector<NodoConPeso>& pesos, int alpha, int beta);

Greedy::Greedy(int alpha, int beta){
	this->alpha = alpha;
	this->beta = beta;
}

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

	while(!pesos.empty()){

		int nodo = pickOne(pesos, this->alpha, this->beta);

		//cambio a partir de la correccion de Diego: para cada nodo sacado en orden segun su peso, su menor peso inicial es infinito.
		float menorPeso = std::numeric_limits<float>::max();
		int indicePariticion = 0;

		for(int l = 0; l < cant_particiones; l++){
			float pesoEnL = peso_intraparticion(nodo, g, particiones[l]);
			if( pesoEnL <= menorPeso){
				indicePariticion = l;
				menorPeso = pesoEnL;
			}
		}

		conjuntoNodos.erase(nodo);
		particiones[indicePariticion].insert(nodo);
		posicionDelNodo[nodo]=indicePariticion+1;
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

int Greedy::getPeso(){
	return this->peso_de_solucion;
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

int peso_de_particion(Particion& particion, int k, const Grafo& g){
	int sumaPesos = 0;
	for(int i = 0; i < particion.size(); i++){
		for(Conjunto::const_iterator it = particion[i].begin(); it != particion[i].end(); it++){
			sumaPesos += g[i][(*it)];
		}
	}
	return sumaPesos;
}

//la idea es agarrar beta% de nodos aleatorios, ordenarlos, y luego sacar uno entre los mejores alpha% de ellos
int pickOne(vector<NodoConPeso>& pesos, int alpha, int beta){
	srand ( clock() );

	//elijo beta% valores del total para el RCL
	vector< pair<NodoConPeso, int> > rcl;
	int cantidad_rcl = ceil((float)beta/100 * (float)pesos.size());
	rcl.reserve(cantidad_rcl);

	for(int i = 0; i < cantidad_rcl; i++){
		int indice = rand() % cantidad_rcl;
		rcl.push_back(make_pair(pesos[indice], indice));
	}

	//ordeno el RCL de mayor a menor en peso de nodo
	sort(rcl.begin(), rcl.end(), ComparacionPesoTupla());

	//elijo uno entre los mejores alpha% del rcl
	int limit = ceil((float)alpha/100 * (float)cantidad_rcl);
	int index = rand() % limit;
	int nodo = rcl[index].first.nodo;

	pesos.erase(pesos.begin()+rcl[index].second);

	return nodo;
}
