#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

typedef vector< vector <float> > grafo;


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

int main (){
	int cant_nodos, cant_aristas, cant_particiones, nodo_u, nodo_v;
	float peso;

	cin >> cant_nodos;
	cin >> cant_aristas;
	cin >> cant_particiones;

	grafo g(cant_nodos, vector<float>(cant_nodos, 0));
	
	for(int i = 0; i < cant_aristas; i++){
		cin >> nodo_u;
		cin >> nodo_v;
		cin >> peso;
		nodo_u--;
		nodo_v--;
		g[nodo_u][nodo_v] = peso;
		g[nodo_v][nodo_u] = peso;
	}

	vector< set <int> > particiones(cant_particiones, set<int>());

	set<int> conjuntoNodos;

	for(int nodo = 0; nodo < cant_nodos; nodo++){
		conjuntoNodos.insert(nodo);
	}

	vector<NodoConPeso> pesos;
	for(int nodo = 0; nodo < cant_nodos; nodo++) {
		pesos.push_back(NodoConPeso(nodo, peso_intraparticion(nodo, g, conjuntoNodos)));
	}

	sort(pesos.begin(), pesos.end(), ComparacionPorPeso());

	vector<int> posicionDelNodo(cant_nodos,0);

	for(int nodo = 0; nodo < pesos.size(); nodo++){

		float menorPeso = peso_intraparticion(pesos[nodo].nodo, g, conjuntoNodos);
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
	
	for(int i = 0; i < cant_nodos-1; i++){
		cout << posicionDelNodo[i] << " ";
	}
	cout << posicionDelNodo[cant_nodos-1] << endl;

	return 0;

}
