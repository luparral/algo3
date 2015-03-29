#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

using namespace std;

typedef vector< vector <float> > grafo;

void show(grafo& grafo, int cant_nodos){
	for(int i = 0; i < cant_nodos; i++){
		cout << "i: " << i << " [";
		for(int j = 0; j < grafo[i].size(); j++){
			cout << grafo[i][j];
			if(j!=grafo[i].size()-1) cout << " ";
		}
		cout << "]" << endl;
	}
}

void showParticiones(vector< set<int> >& particiones){
	for(int i = 0; i < particiones.size(); i++){
		cout << i << ": [";
		for(set<int>::const_iterator it = particiones[i].begin(); it != particiones[i].end(); it++){
			cout << (*it);
		}
		cout << "]" << endl;
	}
}


int peso_intraparticion(int i, grafo& g, const set<int>& particion){
	if(particion.empty()){
		return 0;
	} else {
		int sumaPesos = 0;
		for(set<int>::const_iterator it = particion.begin(); it != particion.end(); it++){
			sumaPesos += g[i][(*it)];
		}
		return sumaPesos;
	}
}

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

int pickOne(vector<NodoConPeso>& pesos, float alpha){
	float limit = ceil(alpha * (float)pesos.size());
	//cout << ceil(alpha * (float)(pesos.size() - 1)) << endl;

	int index = rand() % (int)limit;

	int nodo = pesos[index].nodo;
	pesos.erase(pesos.begin()+index);
	return nodo;
}

vector<int> ejercicio3 (int cant_nodos, int cant_aristas, int cant_particiones, grafo g){

	//inicializo particiones vacias
	vector< set <int> > particiones(cant_particiones, set<int>());

	//creo un set en el cual calcular los pesos.
	set<int> conjuntoNodos;

	//pongo todos los vertices en el conjuntoNodos
	for(int nodo = 0; nodo < cant_nodos; nodo++){
		conjuntoNodos.insert(nodo);
	}

	//obtengo todos los nodos con sus respectivos pesos de su intraparticion
	//Definicion: el peso de un nodo, es la suma del peso de sus aristas
	vector<NodoConPeso> pesos;
	for(int nodo = 0; nodo < cant_nodos; nodo++) {
		pesos.push_back(NodoConPeso(nodo, peso_intraparticion(nodo, g, conjuntoNodos)));
	}

	sort(pesos.begin(), pesos.end(), ComparacionPorPeso());

	vector<int> posicionDelNodo(cant_nodos,0);
	//para cada nodo busco particion l en la que tiene menor peso
	while(!pesos.empty()){

		//elijo un nodo con un parametro alpha (RCL)
		int nodo = pickOne(pesos, 0.25);
		float menorPeso = peso_intraparticion(nodo, g, conjuntoNodos);
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
	
	return posicionDelNodo;
}
