#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <cmath>

using namespace std;

//Typedefs
typedef float                Peso;
typedef vector<int>			 Nodos;
typedef vector<int> 		 Solucion;
typedef vector<int>			 Conjunto;
typedef vector<Conjunto > 	 Particion;
typedef vector<vector<Peso> > Grafo;
typedef unsigned int         Nat;

void limpiarGrafo(Grafo& grafo){
    for(auto i = 0; i < grafo.size(); i++){
        for(auto j = 0; j < grafo.size(); j++){
            grafo[i][j] = 0;
        }
    }
}

int main(int argc, char** argv){
    cout << endl << "Este programa genera instancias validas con k <= n." << endl;

    ofstream output("Instancias.txt");
    cout << "Indique la cantidad de instancias que quiere generar: ";
    Nat cantInstancias = 0;
    cin >> cantInstancias;

    cout << endl << "Indique el tamanio de las instancias que quiere generar (n): ";
    Nat n = 0;
    cin >> n;

    Nat rangoDePesos = round(((2*n)/20) + 20);
    Nat rangoDeConjuntos = n;
    Nat rangoDeAristas = (n * (n-1)) / 2;

    Nat nodo1 = 0;
    Nat nodo2 = 0;
    float peso;
    Grafo grafoActual(n, vector<Peso>(n, 0));

    srand(time(NULL));

    for(auto j = 0; j < cantInstancias; ++j){
        Nat k = (rand() % rangoDeConjuntos) + 1;
        Nat m = (rand() % rangoDeAristas);
        m = (m < n) ? n : m;
        output << n << ' ' << m << ' ' << k << "\n";

        for(auto i = 0; i < m; ++i){
            while(nodo1 == nodo2 || grafoActual[nodo1][nodo2] > 0){
                nodo1 = rand() % n;
                nodo2 = rand() % n;
                cout << nodo1 << ", " << nodo2 << endl;
            }

            peso = (rand() % rangoDePesos) + 1;

            grafoActual[nodo1][nodo2] = peso;
            grafoActual[nodo2][nodo1] = peso;

            output << (nodo1+1) << ' ' << (nodo2+1) << ' ' << peso << "\n";
        }
        limpiarGrafo(grafoActual);
    }
    return 0;
}
