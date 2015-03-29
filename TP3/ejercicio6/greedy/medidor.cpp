#include <utility>
#include <vector>
#include <iostream>
#include <set>
#include "greedy.h"
#include <stdio.h> //printf
#include <time.h>
#include <ctime>

int main (){
    int cant_nodos, cant_aristas, cant_particiones, nodo_u, nodo_v;
    float peso;
    //Medidor
    clock_t t_ini, t_fin;
    float secs;
    int instancias = 0;
    int repeticiones = 0;
    float acum_tiempo = 0;
    float acum_peso = 0;
    cin >> instancias >> repeticiones;

    for(int z = 0; z < instancias; z++){
        cin >> cant_nodos;
        cin >> cant_aristas;
        cin >> cant_particiones;

        Grafo g(cant_nodos, vector<float>(cant_nodos, 0));

        for(int i = 0; i < cant_aristas; i++){
            cin >> nodo_u;
            cin >> nodo_v;
            cin >> peso;
            nodo_u--;
            nodo_v--;
            g[nodo_u][nodo_v] = peso;
            g[nodo_v][nodo_u] = peso;
        }

        float mejor_tiempo = -1;
        float mejor_peso = -1;
        for(int i = 0; i < repeticiones; i++){
            t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo
            Greedy greedy;
            greedy.resolver(g, cant_particiones, cant_nodos);
            peso = greedy.getPeso();

            t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.

            secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".ss
            if(secs < mejor_tiempo || mejor_tiempo == -1) mejor_tiempo = secs;
            if(peso < mejor_peso || mejor_peso == -1) mejor_peso = peso;
        }
        printf("Mejor tiempo: %f, Mejor peso: %f\n", mejor_tiempo, mejor_peso);
        acum_tiempo += mejor_tiempo;
        acum_peso += mejor_peso;

    }
    printf("Tiempo promedio: %f, Peso promedio: %f\n", acum_tiempo/instancias, acum_peso/instancias);

    return 0;
}
