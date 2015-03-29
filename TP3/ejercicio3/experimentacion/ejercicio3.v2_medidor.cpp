#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <time.h>
#include <ctime>
#include <fstream>

using namespace std;

typedef vector< vector <float> > grafo;
//Grafo es un vector de vectores de int, que tiene n (n = cantidad de vertices) en la posición 0, n-1 en la segunda y así.


//Recordar:
//los cant_nodos van numerados del 0 al n-1;

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

int main (){
	int cant_nodos, cant_aristas, cant_particiones, nodo_u, nodo_v;
	float peso;

	cin >> cant_nodos;
	cin >> cant_aristas;
	cin >> cant_particiones;

	//recibo la matriz de adyacencia
	grafo g(cant_nodos, vector<float>(cant_nodos, 0));
	for(int i = 0; i < cant_aristas; i++){
		cin >> nodo_u;
		cin >> nodo_v;
		cin >> peso;
		g[nodo_u][nodo_v] = peso;
		g[nodo_v][nodo_u] = peso;
	}

	int repeticiones = 100;
	double secs;
	clock_t t_ini, t_fin;
	ofstream averagesFile;
	averagesFile.open("promediosEj3.txt", std::ios::app);
	double menorTiempo = -1;
	while(repeticiones > 0){
		t_ini = clock();

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

		//para cada nodo busco particion l en la que tiene menor peso
		for(int nodo = 0; nodo < pesos.size(); nodo++){

			float menorPeso = peso_intraparticion(pesos[nodo].nodo, g, conjuntoNodos);
			int aux = 0;

			for(int l = 0; l < cant_particiones; l++){
				float pesoEnL = peso_intraparticion(pesos[nodo].nodo, g, particiones[l]);
				if( pesoEnL <= menorPeso){
					aux = l;
					menorPeso = pesoEnL;
				}
			}
			conjuntoNodos.erase(pesos[nodo].nodo);
			particiones[aux].insert(pesos[nodo].nodo);
		}
		//showParticiones(particiones);

		t_fin = clock();
		secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
		if(secs < menorTiempo || menorTiempo == -1){
	    		menorTiempo = (double)secs;
	    }

		repeticiones--;
	}

	//cout << 1000.0 * menorTiempo << endl;
    if(averagesFile.is_open()){
    	averagesFile << 1000.0 * menorTiempo << endl;
    }
    cout << "listo" << endl;
    averagesFile.close();
	
	return 0;
	
}
