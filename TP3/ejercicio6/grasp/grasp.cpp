#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include <limits>
#include "greedy.h"
#include "local1.h"
#include "grasp.h"

Grasp::Grasp(int repeticiones, int alpha, int beta){
    this->repeticiones = repeticiones;
    this->alpha = alpha;
    this->beta = beta;
}

Grasp::~Grasp(){};

Solucion Grasp::resolver(const Grafo& g, int n, int k){

    float peso = std::numeric_limits<float>::max();
    float mejor_peso = std::numeric_limits<float>::max();;
    int repeticiones = 0;
    Solucion mejor_solucion;

    while( !Grasp::criterioDeParada(peso, repeticiones) ){

        Greedy greedy(this->alpha, this->beta);
        Solucion solucion = greedy.resolver(g, k, n);

        BusquedaLocal1 local1(solucion);
        solucion = local1.resolver(g, k, n);
        peso = local1.getPeso();

        if(peso < mejor_peso){
            mejor_solucion = solucion;
            mejor_peso = peso;
            repeticiones = 0;
        }
        repeticiones++;
    }

    this->solucion = mejor_solucion;
    this->peso = mejor_peso;

    return mejor_solucion;
}

bool Grasp::criterioDeParada(int peso_actual, int repeticiones){
    if(repeticiones > this->repeticiones || peso_actual == 0){
        return true;
    }
}

void Grasp::imprimir(){
    cout << "------Mejor Solucion------" << endl;
    cout << "Peso: " << this->peso << endl;
    cout << "Solucion:";

    Solucion solucion = this->solucion;

    for(int i = 0; i < solucion.size(); i++){
        cout << solucion[i] << " ";
    }
    cout << endl;
}

float Grasp::getPeso(){
    return this->peso;
}
