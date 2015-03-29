#include <utility>
#include <vector>
#include <iostream>
using namespace std;

typedef float                 Peso;
typedef int                   Nodo;
typedef vector<Nodo>	      conjNodos;
typedef vector<vector<Peso> > Pesos;
typedef vector<conjNodos>     Particion;
typedef vector<int> 		  Solucion;

void swapEnParticion(const int& numConj1, const int& posConj1, const int& numConj2, const int& posConj2, Particion& particion){
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

int pesoDeNodoEnConjunto(const Nodo& nodo, const conjNodos& conjunto, const Pesos& pesos){
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
    /** Organizo la entrada en la partición **/
    int n = solucion.size();
    Particion particionSolucion(k);
    for(int i = 0; i < k; ++i)
        // La operación reserve se realiza para que los sucesivos push_back tengan complejidad O(1)
        particionSolucion[i].reserve(n);

    for(int i = 0; i < n; ++i)
        particionSolucion[solucion[i] - 1].push_back(i);

    if(k > 1){
        /** Inicialización **/
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
            // Si terminé de recorrer el conjActual, debo actualizar el mismo a un conjunto no vacío
            // y el conjComparar a uno no vacío luego del conjActual
            if(posDentroConjActual == cantElemConjActual){
                // Busco el primer conjunto no vacío en las posiciones [numConjActual+1..k-1) y actualizo numConjActual
                numConjActual += 1;
                while( numConjActual < k-1 && particionSolucion[numConjActual].empty())
                    numConjActual++;
                // Si no me pasé del rango válido, fijó variables y busco el proximo conjComparar
                if(numConjActual < k-1){
                    cantElemConjActual = particionSolucion[numConjActual].size();
                    // Busco el primer conjunto no vacío en las posiciones [numConjActual+1..k) y actualizo numConjComparar
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
            // Si terminé de recorrer el conjComparar, debo buscar el próximo a él que no sea vacío
            } else if (posDentroConjComparar == cantElemConjComparar) {
                // Busco el primer conjunto no vacío en las posiciones [numConjComparar+1..k) y actualizo numConjComparar
                numConjComparar += 1;
                while(numConjComparar < k && particionSolucion[numConjComparar].empty())
                    numConjComparar++;
                // Si me fui de rango, no hay más conjs a comparar después de él
                if(numConjComparar < k){
                    cantElemConjComparar = particionSolucion[numConjComparar].size();
                } else {
                    // Podría haber conjuntos no vacíos entre (numConjActual, numConjComparar)
                    posDentroConjActual++;
                    // Busco el primer conjunto no vacío en las posiciones [numConjComparar+1..k) y actualizo numConjComparar
                    numConjComparar = numConjActual + 1;
                    while(numConjComparar < k && particionSolucion[numConjComparar].empty() )
                        numConjComparar++;

                    if(numConjComparar >= k)
                        break;
                }
            }
            // Si o sí estoy en un nuevo conjComparar, arranco desde el principio
            posDentroConjComparar = 0;


            /** Iteración de los conjuntos en busca de swaps favorables **/
            while(posDentroConjActual < cantElemConjActual && posDentroConjComparar < cantElemConjComparar){
                // Guardo los pesos antes y después de swapear y comparo, si no mejoro vuelvo a swapear
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjComparar = pesoDeConjunto(particionSolucion[numConjComparar], pesos);
                swapEnParticion(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                pesoConjActualMod = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjCompararMod = pesoDeConjunto(particionSolucion[numConjComparar], pesos);

                if(pesoConjActual + pesoConjComparar <= pesoConjActualMod + pesoConjCompararMod){
                    // Sólo debo avanzar en el conjComparar ya que no hice swap
                    swapEnParticion(numConjActual, posDentroConjActual, numConjComparar, posDentroConjComparar, particionSolucion);
                    posDentroConjComparar++;
                } else {
                    // Avanzo en ambos ya que hice un swap
                    // Reflejos los cambios hechos en la solución
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
        /** Inicialización **/
        int numConjActual = 0;
        int numConjAgregar = 0;
        int posDentroConjActual = 0;
        int cantElemConjActual = particionSolucion[numConjActual].size();
        Peso pesoConjActual;
        Peso pesoConjAgregar;
        Peso pesoConjActualMod;
        Peso pesoConjAgregarMod;

        while(numConjActual < k){
            // Si ya termine de recorrer el conjunto actual, actualizo valores e índices
            if(posDentroConjActual == cantElemConjActual) {
                // Busco el primer conjunto no vacío en las posiciones [numConjActual+1..k) y actualizo numConjActual
                numConjActual += 1;
                while(numConjActual < k && particionSolucion[numConjActual].empty())
                    numConjActual++;

                posDentroConjActual = 0;

            }
            // Chequeó si me fui de rango, si lo hice termino la ejecución
            if(numConjActual == k){
                break;
            } else {
                // Si no, actualizo variables correctamente
                cantElemConjActual = particionSolucion[numConjActual].size();
                numConjAgregar = ((numConjActual > 0) ? 0 : 1);
            }

            while(numConjAgregar < k){
                // Actualizo los pesos
                pesoConjActual = pesoDeConjunto(particionSolucion[numConjActual], pesos);
                pesoConjAgregar = pesoDeConjunto(particionSolucion[numConjAgregar], pesos);
                pesoConjActualMod = pesoConjActual - pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjActual], pesos);
                pesoConjAgregarMod = pesoConjAgregar + pesoDeNodoEnConjunto(particionSolucion[numConjActual][posDentroConjActual], particionSolucion[numConjAgregar], pesos);

                // Si no disminuyó el peso de la partición, me fijo en el próximo conjunto
                if(pesoConjActual + pesoConjAgregar <= pesoConjActualMod + pesoConjAgregarMod){
                    numConjAgregar++;
                    if(numConjAgregar == numConjActual)
                        numConjAgregar++;
                } else {
                    // Disminuyó, entonces saco el nodo actual del conjunto actual y lo coloco en el conjunto agregar
                    particionSolucion[numConjAgregar].push_back(particionSolucion[numConjActual][posDentroConjActual]);
                    particionSolucion[numConjActual].erase(particionSolucion[numConjActual].begin() + posDentroConjActual);
                    // Reflejo los cambios hechos en la solución
                    solucion[particionSolucion[numConjAgregar].back()] = numConjAgregar + 1;
                    // Debo arrancar desde el principio de todo
                    posDentroConjActual = 0;
                    // Busco el primer conjunto no vacío en las posiciones [0..k) y actualizo numConjComparar
                    numConjActual = 0;
                    while(numConjActual < k && particionSolucion[numConjActual].empty())
                        numConjActual++;

                    if(numConjActual < k)
                        cantElemConjActual = particionSolucion[numConjActual].size();

                    numConjAgregar = ((numConjActual > 0) ? 0 : 1);
                }
            }
            // Terminé de recorrer todos los conjuntos, avanzo al próximo nodo a analizar
            posDentroConjActual++;
        }
    }
}

int main(int argc, char** argv){
    /** Inicialización matriz de pesos **/
    int n, m, k;
	cin >> n >> m >> k;

	Pesos pesos_ij;
	pesos_ij.reserve(n);
	for(int i = 1; i < n; i++){
		vector<Peso> pesos_de_arista_i(n-i, 0);
		pesos_ij.push_back(pesos_de_arista_i);
	}

	for (int i = 0; i < m; i++){
		int x, y;
		Peso p;
		cin >> x >> y >> p;

		if(x < y)
			pesos_ij[x-1][y-x-1] = p;
		else
			pesos_ij[y-1][x-y-1] = p;
	}

	cout << "La solucion original es: " << endl;

    /** Inicialización solución **/
    Solucion solucion(n);
    int valor;
    for(int i = 0; i < n; ++i){
        cin >> valor;
        solucion[i] = valor;
    }

    cout << "El peso de la solucion original es: " << endl << pesoDeSolucion(solucion, k, pesos_ij) << endl;

    busqueda_local_1(solucion, k, pesos_ij);

    cout << "La solucion modificada es: " << endl;
    for(int i = 0; i < solucion.size(); i++)
        cout << solucion[i] << ' ';

    cout << endl << "El peso de la solucion modificada es: " << endl << pesoDeSolucion(solucion, k, pesos_ij) << endl;

    return 0;
}


