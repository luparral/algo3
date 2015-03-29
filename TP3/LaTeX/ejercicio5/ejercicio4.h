#include <utility>
#include <vector>
#include <iostream>
using namespace std;

typedef float                 Peso;
typedef int                   Nodo;
typedef vector<Nodo>	      Nodos;
typedef vector<vector<Peso> > Pesos;
typedef vector<Nodos>         Particion;
typedef vector<int> 		  Solucion;

void swap(const int& numConj1, const int& posConj1, const int& numConj2, const int& posConj2, Particion& particion){
	int temp = particion[numConj1][posConj1];
	particion[numConj1][posConj1] = particion[numConj2][posConj2];
	particion[numConj2][posConj2] = temp;
}

Peso pesoDeArista(Nodo nodo_i, Nodo nodo_j, const Pesos& pesos){
	if(nodo_i < nodo_j)
		return pesos[nodo_i][nodo_j-nodo_i-1];
	else
		return pesos[nodo_j][nodo_i-nodo_j-1];
}

Peso pesoDeConjunto(vector<Nodo>& conjDeNodos, const Pesos& pesos){
	if(conjDeNodos.size() < 2)
		return 0;

	int acum = 0;
	for(int i = 0; i < conjDeNodos.size()-1; i++)
		for(int j = i+1; j < conjDeNodos.size(); j++)
			acum += pesoDeArista(conjDeNodos[i], conjDeNodos[j], pesos);

	return acum;
}

Peso pesoDeParticion(Particion& particion, const Pesos& pesos){
	Peso acum = 0;
	for (int i = 0; i < particion.size(); i++)
		acum += pesoDeConjunto(particion[i], pesos);

	return acum;
}

void print(Particion& particion){
	for(int i = 0; i < particion.size(); i++){
		cout << "[ ";
		for(int j = 0; j < particion[i].size(); j++)
			cout << particion[i][j] + 1 << " " ;

		cout << "] ";
	}
	cout << endl;
}

int pesoDeNodoEnConjunto(const Nodo& nodo, const Nodos& conjunto, const Pesos& pesos){
    int pesoTotal = 0;
    int cantElem = conjunto.size();
    for(int i = 0; i < cantElem; ++i){
        if(nodo != conjunto[i])
            pesoTotal = pesoTotal + pesoDeArista(nodo, conjunto[i], pesos);
    }

    return pesoTotal;
}

void printParticionDeSolucion(const Solucion& solucion, const int& k){
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i)
        particionSolucion[i].reserve(n);

    for(int i = 0; i < n; ++i)
        particionSolucion[solucion[i] - 1].push_back(i);

    print(particionSolucion);
}

int pesoDeSolucion(const Solucion& solucion, const int& k, const Pesos& pesos){
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i)
        particionSolucion[i].reserve(n);

    for(int i = 0; i < n; ++i)
        particionSolucion[solucion[i] - 1].push_back(i);

    return pesoDeParticion(particionSolucion, pesos);
}

void busqueda_local_1(Solucion& solucion, const int& k, const Pesos& pesos){
    /** Organizo la entrada en conjuntos **/
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i)
        particionSolucion[i].reserve(n);

    for(int i = 0; i < n; ++i)
        particionSolucion[solucion[i] - 1].push_back(i);

    if(k > 1){
        /** Inicializaci�n **/
        int n = solucion.size();
        int numConjActual = 0;
        int numConjComparar = 1;
        int cantElemConjActual = particionSolucion[numConjActual].size();
        int cantElemConjComparar = particionSolucion[numConjComparar].size();
        int pesoConjActual;
        int pesoConjComparar;
        int pesoConjActualMod;
        int pesoConjCompararMod;
        int posDentroConjActual = 0;
        int posDentroConjComparar = 0;

        /** Cuerpo **/
        while(numConjActual < k-1){
            /** Calculos de �ndices, tama�os y demas **/
            if(posDentroConjActual == cantElemConjActual){
                numConjActual += 1;
                while( numConjActual < k-1 && particionSolucion[numConjActual].empty())
                    numConjActual++;

                if(numConjActual < k-1){
                    cantElemConjActual = particionSolucion[numConjActual].size();

                    numConjComparar = numConjActual + 1;
                    while(numConjComparar < k && particionSolucion[numConjComparar].empty() )
                        numConjComparar++;

                    if(numConjComparar < k) {
                        cantElemConjComparar = particionSolucion[numConjComparar].size();
                    } else {
                        cantElemConjComparar = 0;
                    }
                    posDentroConjActual = 0;
                } else {
                    numConjActual = k;
                }

            } else if (posDentroConjComparar == cantElemConjComparar) {
                numConjComparar += 1;
                while(numConjComparar < k && particionSolucion[numConjComparar].empty())
                    numConjComparar++;

                if(numConjComparar < k){
                    cantElemConjComparar = particionSolucion[numConjComparar].size();
                } else {
                    posDentroConjActual++;
                    numConjComparar = numConjActual + 1;
                    while(numConjComparar < k && particionSolucion[numConjComparar].empty() )
                        numConjComparar++;
                }
            }
            posDentroConjComparar = 0;


            /** Iteraci�n de los conjuntos en busca de swaps favorables **/
            while(posDentroConjActual < cantElemConjActual && posDentroConjComparar < cantElemConjComparar){
                // Guardo los pesos antes y despu�s de swapear y comparo, si no mejoro vuelvo a swapear
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjComparar = pesoDeConjunto(particionSolucion[numConjComparar], pesos);
                swap(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                pesoConjActualMod = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjCompararMod = pesoDeConjunto(particionSolucion[numConjComparar], pesos);

                if(pesoConjActual + pesoConjComparar <= pesoConjActualMod + pesoConjCompararMod){
                    swap(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                    posDentroConjComparar++;
                } else {
                    solucion[particionSolucion[numConjActual][posDentroConjActual]] = numConjActual + 1;
                    solucion[particionSolucion[numConjComparar][posDentroConjComparar]] = numConjComparar + 1;
                    posDentroConjActual++;
                    posDentroConjComparar++;
                }
            }
        }
    }
}

void busqueda_local_2(Solucion& solucion, const int& k, const Pesos& pesos){
    /** Organizo la entrada en conjuntos **/
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i)
        particionSolucion[i].reserve(n);

    for(int i = 0; i < n; ++i)
        particionSolucion[solucion[i] - 1].push_back(i);

    if(k > 1){
        /** Inicializaci�n **/
        int numConjActual = 0;
        int numConjAgregar = 0;
        int posDentroConjActual = 0;
        int cantElemConjActual = particionSolucion[numConjActual].size();
        int pesoConjActual;
        int pesoConjAgregar;
        int pesoConjActualMod;
        int pesoConjAgregarMod;

        while(numConjActual < k){
            if(posDentroConjActual == cantElemConjActual) {
                numConjActual += 1;
                while(numConjActual < k && particionSolucion[numConjActual].empty())
                    numConjActual++;

                cantElemConjActual = particionSolucion[numConjActual].size();
                posDentroConjActual = 0;
            }
            if(numConjActual == k){
                numConjAgregar = k;
            } else {
                numConjAgregar = ((numConjActual > 0) ? 0 : 1);
            }

            while(numConjAgregar < k){
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjAgregar = pesoDeConjunto(particionSolucion[numConjAgregar], pesos);
                pesoConjActualMod = pesoConjActual - pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjActual], pesos);
                pesoConjAgregarMod = pesoConjAgregar + pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjAgregar], pesos);

                if(pesoConjActual + pesoConjAgregar <= pesoConjActualMod + pesoConjAgregarMod){
                    numConjAgregar++;
                    if(numConjAgregar == numConjActual)
                        numConjAgregar++;
                } else {
                    particionSolucion[numConjAgregar].push_back(particionSolucion[numConjActual][posDentroConjActual]);
                    particionSolucion[numConjActual].erase(particionSolucion[numConjActual].begin() + posDentroConjActual);

                    solucion[particionSolucion[numConjAgregar].back()] = numConjAgregar + 1;
                    posDentroConjActual = 0;
                    numConjActual = 0;
                    while(numConjActual < k && particionSolucion[numConjActual].empty())
                        numConjActual++;

                    cantElemConjActual = particionSolucion[numConjActual].size();

                    numConjAgregar = ((numConjActual > 0) ? 0 : 1);
                }
            }
            posDentroConjActual++;
        }
    }
}
