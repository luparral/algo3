#include <vector>
#include "ejercicio3.h"
#include "ejercicio4.h"

typedef vector< vector<float> > Matriz;
typedef vector<vector<Peso> > Pesos;

int main (){

    int n, m, k;

    //cout << "Ingrese cantidad de nodos, aristas y particiones: ";
    cin >> n >> m >> k;

    Pesos pesos_ij;
    pesos_ij.reserve(n);
    for(int i = 1; i < n; i++){
        vector<Peso> pesos_de_arista_i(n-i, 0);
        pesos_ij.push_back(pesos_de_arista_i);
    }

    int u, v;
    float peso;
    //recibo la matriz de adyacencia
    Matriz matriz(n, vector<float>(n, 0));
    for(int i = 0; i < m; i++){
        //cout << "Ingrese 2 nodos a asignar adyacencia, y luego el peso: ";
        cin >> u >> v >> peso;


        if(u < v){
            pesos_ij[u-1][v-u-1] = peso;
        }
        else{
            pesos_ij[v-1][u-v-1] = peso;
        }

        u--;
        v--;
        matriz[u][v] = peso;
        matriz[v][u] = peso;
    }

    /** Inicializaci�n soluci�n **/
    vector<int> solucion(n);
    int valor;
    for(int i = 0; i < n; ++i){
        cin >> valor;
        solucion[i] = valor;
    }

    solucion = ejercicio3(n, m, k, matriz);
    vector<int> solucionBuffer = solucion;

    cout << "Greedy: ";
    for(int i = 0; i<solucion.size(); i++) {
        cout << solucion[i] << " ";
    }

    cout << "Peso: " << pesoDeSolucion(solucion, k, pesos_ij) << endl;

    //busqueda_local_1(solucion, k, pesos_ij);

    cout << "Local 1: ";
    for(int i = 0; i<solucion.size(); i++) {
        cout << solucion[i] << " ";
    }

    cout << "Peso: " << pesoDeSolucion(solucion, k, pesos_ij) << endl;

    busqueda_local_2(solucionBuffer, k, pesos_ij);

    cout << "Local 2: ";
    for(int i = 0; i<solucionBuffer.size(); i++) {
        cout << solucionBuffer[i] << " ";
    }

    cout << "Peso: " << pesoDeSolucion(solucionBuffer, k, pesos_ij) << endl;

    return 0;
}
