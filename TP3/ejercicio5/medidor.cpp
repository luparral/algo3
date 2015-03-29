#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "grasp.h"
#include <limits>
#include <stdio.h> //printf
#include <time.h>
#include <ctime>

using namespace std;

int main(){

    int n, m, k, u, v;
    float peso;
    Solucion sol;

    //Medidor
    clock_t t_ini, t_fin;
    float secs;
    int instancias = 0;
    int repeticiones = 0;
    float acum_tiempo = 0;
    float acum_peso = 0;
    cin >> instancias >> repeticiones;
    int alpha = 0;
    int beta = 0;
    cin >> alpha >> beta;
    //End

    for(int z = 0; z < instancias; z++){

        //Carga de datos
        cin >> n;
        cin >> m;
        cin >> k;

        Grafo g(n, vector<float>(n, 0));

        for(int i = 0; i < m; i++){
            cin >> u;
            cin >> v;
            cin >> peso;
            u--;
            v--;
            g[u][v] = peso;
            g[v][u] = peso;
        }

        float mejor_tiempo = std::numeric_limits<double>::max();
        float mejor_peso = std::numeric_limits<float>::max();
        //Repeticion de instancias
        for(int j = 0; j < repeticiones; j++){

            t_ini = clock();

            Grasp grasp(100, alpha, beta);
            grasp.resolver(g, n, k);

            t_fin = clock();

            float tiempo = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

            peso = grasp.getPeso();
            if(tiempo < mejor_tiempo) mejor_tiempo = tiempo;
            if(peso < mejor_peso) mejor_peso = peso;
        }
        printf("Mejor tiempo: %f, Mejor peso: %f\n", mejor_tiempo, mejor_peso);
        acum_tiempo += mejor_tiempo;
        acum_peso += mejor_peso;

    }
    printf("Tiempo promedio: %f, Peso promedio: %f\n", acum_tiempo/instancias, acum_peso/instancias);

    return 0;
}
