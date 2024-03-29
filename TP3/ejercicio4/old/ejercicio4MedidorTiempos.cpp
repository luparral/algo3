#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <limits>
#include <iomanip>
using namespace std;

// Typedefs
typedef float                 Peso;
typedef int                   Nodo;
typedef vector<int>	          conjNodos;
typedef vector<vector<Peso> > Grafo;
typedef vector<conjNodos>     Particion;
typedef vector<int> 		  Solucion;

void swapEnParticion(const int& numConj1, const int& posConj1, const int& numConj2, const int& posConj2, Particion& particion){
	int temp = particion[numConj1][posConj1];
	particion[numConj1][posConj1] = particion[numConj2][posConj2];
	particion[numConj2][posConj2] = temp;
}

Peso pesoDeArista(const int& nodo1, const int& nodo2, const Grafo& grafo){
	return grafo[nodo1][nodo2];
}

Peso pesoDeConjunto(const conjNodos& conjDeNodos, const Grafo& grafo){
	if(conjDeNodos.size() < 2){
		return 0;
	}

	Peso acum = 0;
	for(int i = 0; i < (conjDeNodos.size()-1); i++){
		for(int j = i+1; j < conjDeNodos.size(); j++){
			acum += pesoDeArista(conjDeNodos[i], conjDeNodos[j], grafo);
		}
	}

	return acum;
}

Peso pesoDeParticion(const Particion& particion, const Grafo& grafo){
	Peso acum = 0;
	for (int i = 0; i < particion.size(); i++){
		acum += pesoDeConjunto(particion[i], grafo);
	}

	return acum;
}

void print(const Particion& particion){
	for(int i = 0; i < particion.size(); i++){
		cout << "[ ";
		for(int j = 0; j < particion[i].size(); j++){
			cout << particion[i][j] + 1 << " " ;
        }

		cout << "] ";
	}
	cout << endl;
}

Peso pesoDeNodoEnConjunto(const Nodo& nodo, const conjNodos& conjunto, const Grafo& grafo){
    Peso pesoTotal = 0;
    int cantElem = conjunto.size();
    for(int i = 0; i < cantElem; ++i){
        if(nodo != conjunto[i]){
            pesoTotal = pesoTotal + pesoDeArista(nodo, conjunto[i], grafo);
        }
    }

    return pesoTotal;
}

void printParticionDeSolucion(const Solucion& solucion, const int& k){
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i){
        particionSolucion[i].reserve(n);
    }

    for(int i = 0; i < n; ++i){
        particionSolucion[solucion[i] - 1].push_back(i);
    }

    print(particionSolucion);
}

Peso pesoDeSolucion(const Solucion& solucion, const int& k, const Grafo& grafo){
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i){
        particionSolucion[i].reserve(n);
    }

    for(int i = 0; i < n; ++i){
        particionSolucion[solucion[i] - 1].push_back(i);
    }

    return pesoDeParticion(particionSolucion, grafo);
}

void busqueda_local_1(Solucion& solucion, const int& k, const Grafo& grafo){
    /** Organizo la entrada en la partici�n **/
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i){
        // La operaci�n reserve se realiza para que los sucesivos push_back tengan complejidad O(1)
        particionSolucion[i].reserve(n);
    }

    for(int i = 0; i < n; ++i){
        particionSolucion[solucion[i] - 1].push_back(i);
    }

    if(k > 1){
        /** Inicializaci�n **/
        int numConjActual = 0;
        int numConjComparar = 1;
        int cantElemConjActual = particionSolucion[numConjActual].size();
        int cantElemConjComparar = particionSolucion[numConjComparar].size();
        Peso pesoConjActual;
        Peso pesoConjComparar;
        Peso pesoConjActualMod;
        Peso pesoConjCompararMod;
        int posDentroConjActual = 0;
        int posDentroConjComparar = 0;

        /** Cuerpo **/
        while(numConjActual < k-1){
            // Si termin� de recorrer el conjActual, debo actualizar el mismo a un conjunto no vac�o
            // y el conjComparar a uno no vac�o luego del conjActual
            if(posDentroConjActual == cantElemConjActual){
                // Busco el primer conjunto no vac�o en las posiciones [numConjActual+1..k-1) y actualizo numConjActual
                numConjActual += 1;
                while( numConjActual < k-1 && particionSolucion[numConjActual].empty())
                    numConjActual++;
                // Si no me pas� del rango v�lido, fij� variables y busco el proximo conjComparar
                if(numConjActual < k-1){
                    cantElemConjActual = particionSolucion[numConjActual].size();
                    // Busco el primer conjunto no vac�o en las posiciones [numConjActual+1..k) y actualizo numConjComparar
                    numConjComparar = numConjActual + 1;
                    while(numConjComparar < k && particionSolucion[numConjComparar].empty() )
                        numConjComparar++;
                    // Si el conjComparar no se fue de rango, continuo y actualizo variables
                    if(numConjComparar < k) {
                        cantElemConjComparar = particionSolucion[numConjComparar].size();
                    } else {
                        break;
                    }
                    // Estoy en un conj actual nuevo, debo arrancar desde 0
                    posDentroConjActual = 0;
                } else {
                    break;
                }
            // Si termin� de recorrer el conjComparar, debo buscar el pr�ximo a �l que no sea vac�o
            } else if (posDentroConjComparar == cantElemConjComparar) {
                // Busco el primer conjunto no vac�o en las posiciones [numConjComparar+1..k) y actualizo numConjComparar
                numConjComparar += 1;
                while(numConjComparar < k && particionSolucion[numConjComparar].empty())
                    numConjComparar++;
                // Si me fui de rango, no hay m�s conjs a comparar despu�s de �l
                if(numConjComparar < k){
                    cantElemConjComparar = particionSolucion[numConjComparar].size();
                } else {
                    // Podr�a haber conjuntos no vac�os entre (numConjActual, numConjComparar)
                    posDentroConjActual++;
                    // Busco el primer conjunto no vac�o en las posiciones [numConjComparar+1..k) y actualizo numConjComparar
                    numConjComparar = numConjActual + 1;
                    while(numConjComparar < k && particionSolucion[numConjComparar].empty() )
                        numConjComparar++;

                    if(numConjComparar >= k){
                        break;
                    } else {
                    	cantElemConjComparar = particionSolucion[numConjComparar].size();
                    }
                }
            }
            // Si o s� estoy en un nuevo conjComparar, arranco desde el principio
            posDentroConjComparar = 0;


            /** Iteraci�n de los conjuntos en busca de swaps favorables **/
            while(posDentroConjActual < cantElemConjActual && posDentroConjComparar < cantElemConjComparar){
                // Guardo los pesos antes y despu�s de swapear y comparo, si no mejoro vuelvo a swapear
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], grafo);
                pesoConjComparar = pesoDeConjunto(particionSolucion[numConjComparar], grafo);
                swapEnParticion(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                pesoConjActualMod = pesoDeConjunto(particionSolucion[numConjActual], grafo);
                pesoConjCompararMod = pesoDeConjunto(particionSolucion[numConjComparar], grafo);

                if(pesoConjActual + pesoConjComparar <= pesoConjActualMod + pesoConjCompararMod){
                    // S�lo debo avanzar en el conjComparar ya que no hice swapEnParticion
                    swapEnParticion(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                    posDentroConjComparar++;
                } else {
                    // Avanzo en ambos ya que hice un swap
                    // Reflejos los cambios hechos en la soluci�n
                    solucion[particionSolucion[numConjActual][posDentroConjActual]] = numConjActual + 1;
                    solucion[particionSolucion[numConjComparar][posDentroConjComparar]] = numConjComparar + 1;
                    posDentroConjActual++;
                    posDentroConjComparar++;
                }
            }
        }
    }
}

void busqueda_local_2(Solucion& solucion, const int& k, const Grafo& grafo){
    /** Organizo la entrada en conjuntos **/
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i){
        particionSolucion[i].reserve(n);
    }

    for(int i = 0; i < n; ++i){
        particionSolucion[solucion[i] - 1].push_back(i);
    }

    if(k > 1){
        /** Inicializaci�n **/
        int numConjActual = 0;
        int numConjAgregar = 0;
        int posDentroConjActual = 0;
        int cantElemConjActual = particionSolucion[numConjActual].size();
        Peso pesoConjActual;
        Peso pesoConjAgregar;
        Peso pesoConjActualMod;
        Peso pesoConjAgregarMod;

        while(numConjActual < k){
            // Si ya termine de recorrer el conjunto actual, actualizo valores e �ndices
            if(posDentroConjActual == cantElemConjActual) {
                // Busco el primer conjunto no vac�o en las posiciones [numConjActual+1..k) y actualizo numConjActual
                numConjActual += 1;
                while(numConjActual < k && particionSolucion[numConjActual].empty())
                    numConjActual++;

                posDentroConjActual = 0;

            }
            // Cheque� si me fui de rango, si lo hice termino la ejecuci�n
            if(numConjActual == k){
                break;
            } else {
                // Si no, actualizo variables correctamente
                cantElemConjActual = particionSolucion[numConjActual].size();
                numConjAgregar = ((numConjActual > 0) ? 0 : 1);
            }

            while(numConjAgregar < k){
                // Actualizo los pesos
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], grafo);
                pesoConjAgregar = pesoDeConjunto(particionSolucion[numConjAgregar], grafo);
                pesoConjActualMod = pesoConjActual - pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjActual], grafo);
                pesoConjAgregarMod = pesoConjAgregar + pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjAgregar], grafo);

                // Si no disminuy� el peso de la partici�n, me fijo en el pr�ximo conjunto
                if(pesoConjActual + pesoConjAgregar <= pesoConjActualMod + pesoConjAgregarMod){
                    numConjAgregar++;
                    if(numConjAgregar == numConjActual)
                        numConjAgregar++;
                } else {
                    // Disminuy�, entonces saco el nodo actual del conjunto actual y lo coloco en el conjunto agregar
                    particionSolucion[numConjAgregar].push_back(particionSolucion[numConjActual][posDentroConjActual]);
                    particionSolucion[numConjActual].erase(particionSolucion[numConjActual].begin() + posDentroConjActual);
                    // Reflejo los cambios hechos en la soluci�n
                    solucion[particionSolucion[numConjAgregar].back()] = numConjAgregar + 1;
                    // Debo arrancar desde el principio de todo
                    posDentroConjActual = 0;
                    // Busco el primer conjunto no vac�o en las posiciones [0..k) y actualizo numConjComparar
                    numConjActual = 0;
                    while(numConjActual < k && particionSolucion[numConjActual].empty())
                        numConjActual++;

                    if(numConjActual < k)
                        cantElemConjActual = particionSolucion[numConjActual].size();

                    numConjAgregar = ((numConjActual > 0) ? 0 : 1);
                }
            }
            // Termin� de recorrer todos los conjuntos, avanzo al pr�ximo nodo a analizar
            posDentroConjActual++;
        }
    }
}

void limpiarGrafo(Grafo& grafo){
    auto n = grafo.size();
    for(auto i = 0; i < n; i++){
        for(auto j = 0; j < i; j++){
            grafo[i][j] = 0;
            grafo[j][i] = 0;
        }
    }
}

int main(int argc, char** argv){
    ifstream input("InstanciasConSolucion.txt");
    int cantInstancias = 0;
    input >> cantInstancias;

    int n;
    input >> n;
    int m, k;
    Peso peso;
    int x, y;
    Grafo grafo(n, vector<Peso>(n,0));
    Solucion solucion1(n, 0);
    Solucion solucion2;
    double minTimeH1 = numeric_limits<double>::max();
    double minTimeH2 = numeric_limits<double>::max();

    for(int x = 0; x < cantInstancias; ++x){
        input >> n >> m >> k;
        // Tomo datos del grafo
        for (int i = 0; i < m; i++){
            input >> x >> y >> peso;

            grafo[x-1][y-1] = peso;
            grafo[y-1][x-1] = peso;
        }

        // Tomo soluci�n de input (va a ser una greedy)
        for(int i = 0; i < n; i++){
            input >> solucion1[i];
        }
        solucion2 = solucion1;

        // Medici�n heur�stica 1
        clock_t begin = clock();
        busqueda_local_1(solucion1, k, grafo);
        clock_t end = clock();
        double elapsedTime = (double(end - begin)) / CLOCKS_PER_SEC;
        if(elapsedTime < minTimeH1){
            minTimeH1 = elapsedTime;
        }

        // Medicion Heuristica 2
        begin = clock();
        busqueda_local_2(solucion2, k, grafo);
        end = clock();
        elapsedTime = (double(end - begin)) / CLOCKS_PER_SEC;
        if(elapsedTime < minTimeH2){
            minTimeH2 = elapsedTime;
        }

        limpiarGrafo(grafo);
    }

    cout << "El tiempo minimo para Heuristica 1 es: " << setprecision(4) << minTimeH1 << " segs" << endl;
    cout << "El tiempo minimo para Heuristica 2 es: " << setprecision(4) << minTimeH2 << " segs" << endl;

    return 0;
}
