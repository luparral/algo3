#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
#include "local2.h"

using namespace std;


//Typedefs

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;
typedef set<int> Conjunto;
typedef vector<Conjunto> Particion;

//Prototipado de funciones

Particion convertir_solucion_en_particion(const Solucion& sol, int cant_particiones);
float peso_de_particion(const Particion& part, int k, const Grafo& g);
void imprimir_particion(Particion& particion);
void mover_nodo(int nodo_i, int conjunto_nodo_j, int conjunto_j, Particion& particion_actual);

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


float peso_de_particion(const Particion& part, int k, const Grafo& g){
	float sumaPesos = 0;
	for(int i = 0; i < k; i++){
		for(Conjunto::const_iterator it = part[i].begin(); it != part[i].end(); it++){
			sumaPesos += g[i][(*it)];
		}
	}
	return sumaPesos;
}

BusquedaLocal2::BusquedaLocal2(const Solucion& sol){
	this->solucion_inicial = sol;
	this->mejor_vecino = sol;
}

BusquedaLocal2::~BusquedaLocal2(){}

Solucion BusquedaLocal2::resolver(const Grafo& g, int cant_particiones, int cant_nodos){
	Particion particion_actual = convertir_solucion_en_particion(this->solucion_inicial, cant_particiones);
	Particion mejor_particion = convertir_solucion_en_particion(this->solucion_inicial, cant_particiones);
	for(int nodo_i = 0; nodo_i < cant_nodos; nodo_i++){
		int conjunto_nodo_i = this->solucion_inicial[nodo_i]-1;
		for(int j = 0; j < cant_particiones; j++){
			if(conjunto_nodo_i != j){
				//me muevo al vecino
				mover_nodo(nodo_i, conjunto_nodo_i, j, particion_actual);
				if(peso_de_particion(particion_actual, cant_particiones, g) < peso_de_particion(mejor_particion, cant_particiones, g)){
					mejor_particion = particion_actual;

				}
				//restablezco al inicial
				mover_nodo(nodo_i, j, conjunto_nodo_i, particion_actual);
			}

		}
	}

	this->peso_mejor_vecino = peso_de_particion(mejor_particion, cant_particiones, g);
	this->mejor_vecino = convertir_particion_en_solucion(mejor_particion, cant_nodos);
	return this->mejor_vecino;
}

void mover_nodo(int nodo_i, int conjunto_nodo_i, int j, Particion& particion_actual){
	particion_actual[conjunto_nodo_i].erase(nodo_i);
	particion_actual[j].insert(nodo_i);

}

void BusquedaLocal2::imprimir_solucion(bool imprimir_peso, int cant_particiones, const Grafo& g) const{
	if(imprimir_peso){
		Particion particion = convertir_solucion_en_particion(this->mejor_vecino, cant_particiones);
		cout << peso_de_particion(particion, cant_particiones, g) << endl;
	}

	for(int i = 0; i < this->mejor_vecino.size()-1; i++){
		cout << this->mejor_vecino[i] << " ";
	}
	cout << this->mejor_vecino[this->mejor_vecino.size()-1] << endl;

}

float BusquedaLocal2::getPeso(){
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
