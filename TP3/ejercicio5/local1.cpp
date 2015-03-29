#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
#include "local1.h"

using namespace std;


//Typedefs

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;
typedef set<int> Conjunto;
typedef vector<Conjunto> Particion;

//Prototipado de funciones

Particion convertir_solucion_en_particion(const Solucion& sol, int cant_particiones);
int peso_de_particion(const Particion& part, int k, const Grafo& g);
void imprimir_particion(Particion& particion);
void swapear_nodos(int nodo_i, int conjunto_nodo_j, int nodo_j, int conjunto_nodo_i, Particion& particion_actual);


Particion convertir_solucion_en_particion(const Solucion& sol, int cant_particiones){

	Particion particion(cant_particiones, Conjunto());
	for(int i = 0; i < sol.size(); i++){
		particion[sol[i]-1].insert(i);
	}
	return particion;

}

Solucion convertir_particion_en_solucion(const Particion& particion, int cant_nodos){
	Solucion solucion(cant_nodos, -1);
	for(int k = 0; k < particion.size(); k++){
		for(Conjunto::const_iterator it = particion[k].begin(); it != particion[k].end(); it++){
			solucion[(*it)] = k+1;
		}
	}
	return solucion;
}


int peso_de_particion(const Particion& part, int k, const Grafo& g){
	int sumaPesos = 0;
	for(int i = 0; i < k; i++){
		for(Conjunto::const_iterator it = part[i].begin(); it != part[i].end(); it++){
			sumaPesos += g[i][(*it)];
		}
	}
	return sumaPesos;
}

BusquedaLocal1::BusquedaLocal1(const Solucion& sol){
	this->solucion_inicial = sol;
	this->mejor_vecino = sol;
}

BusquedaLocal1::~BusquedaLocal1(){}

Solucion BusquedaLocal1::resolver(const Grafo& g, int cant_particiones, int cant_nodos){
	Particion particion_actual = convertir_solucion_en_particion(this->solucion_inicial, cant_particiones);
	Particion mejor_particion = convertir_solucion_en_particion(this->solucion_inicial, cant_particiones);
	for(int nodo_i = 0; nodo_i < cant_nodos; nodo_i++){
		int conjunto_nodo_i = this->solucion_inicial[nodo_i]-1;
		for(int nodo_j = nodo_i+1; nodo_j < cant_nodos; nodo_j++){
			int conjunto_nodo_j = this->solucion_inicial[nodo_j]-1;
			if(conjunto_nodo_i != conjunto_nodo_j){
				//me muevo al vecino
				swapear_nodos(nodo_i, conjunto_nodo_j, nodo_j, conjunto_nodo_i, particion_actual);
				if(peso_de_particion(particion_actual, cant_particiones, g) < peso_de_particion(mejor_particion, cant_particiones, g)){
					mejor_particion = particion_actual;

				}
				//restablezco al inicial
				swapear_nodos(nodo_i, conjunto_nodo_i, nodo_j, conjunto_nodo_j, particion_actual);
			}

		}
	}

	this->peso_mejor_vecino = peso_de_particion(mejor_particion, cant_particiones, g);
	this->mejor_vecino = convertir_particion_en_solucion(mejor_particion, cant_nodos);
	return this->mejor_vecino;
}

void swapear_nodos(int nodo_i, int conjunto_nodo_j, int nodo_j, int conjunto_nodo_i, Particion& particion_actual){
	particion_actual[conjunto_nodo_i].erase(nodo_i);
	particion_actual[conjunto_nodo_i].insert(nodo_j);

	particion_actual[conjunto_nodo_j].erase(nodo_j);
	particion_actual[conjunto_nodo_j].insert(nodo_i);

}

void BusquedaLocal1::imprimir_solucion(bool imprimir_peso, int cant_particiones, const Grafo& g) const{
	if(imprimir_peso){
		Particion particion = convertir_solucion_en_particion(this->mejor_vecino, cant_particiones);
		cout << peso_de_particion(particion, cant_particiones, g) << endl;
	}

	for(int i = 0; i < this->mejor_vecino.size()-1; i++){
		cout << this->mejor_vecino[i] << " ";
	}
	cout << this->mejor_vecino[this->mejor_vecino.size()-1] << endl;

}

int BusquedaLocal1::getPeso(){
	return this->peso_mejor_vecino;
}

void imprimir_particion(Particion& particion){
	for(int i = 0; i < particion.size(); i++){
		cout <<"[";
		for(Conjunto::const_iterator it = particion[i].begin(); it != particion[i].end(); it++){
			cout << (*it) << " ";
		}
		cout << "]" << endl;
	}

}
