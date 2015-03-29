#include <fstream>  // ifstream/ofstream/fstream
#include <iostream> // cout/cin
#include <vector>   // vector
#include <utility>  // pair
#include <list> // list
#include <queue> // queue
#include <stdlib.h> // rand
#include <time.h>

using namespace std;

typedef int nodo;
typedef int peso;
typedef vector<vector<peso> > grafo;
typedef pair<peso, nodo> pesoPredecesor;
typedef vector<pesoPredecesor> vectorPesoPredecesor;
typedef pair<nodo, nodo> parNodo;
typedef pair<grafo, bool> tuplaSolucion;
typedef pair<parNodo, bool> tuplaCiclo;
typedef vector<nodo> predecesores;
typedef list<nodo> camino;

//Devuelve matrices cuadradas de tam * tam
void falseAll(vector<vector<bool>>& m)
{
    auto n = m.size();
    for(auto i = 0; i < n; ++i)
    {
        for(auto j = 0; j < i; ++j)
        {
            m[i][j] = false;
            m[j][i] = false;
        }
    }
}

int main(int argc, char** argv)
{
    cout << "Indique cantidad de redes a generar: ";
    unsigned int instancias = 0;
    cin >> instancias;
    unsigned int cantEquipos = 0;
    cout << "Indique cantidad de equipos de las redes a generar: ";
    cin >> cantEquipos;

    nodo equipo1;
    nodo equipo2;
    unsigned int cantMaxAristasPorInstancia = 0;
    unsigned int cantAristasPorInstancia = 0;
    vector<vector<bool>> unidos(cantEquipos, vector<bool>(cantEquipos,false));
    srand(time(NULL));

    ofstream output("C:/Emanuel/Facu/Algo 3/Git/AlgoIII/TP2/ejercicio_3/InstanciasPeorCaso.txt", ios::out);

    for(auto i = 0; i < instancias; i++)
    {
        cantMaxAristasPorInstancia = (cantEquipos * (cantEquipos - 1)) / 2;
        cantAristasPorInstancia = rand() % cantMaxAristasPorInstancia + 1;
        falseAll(unidos);
        output << cantEquipos << ' ' << cantMaxAristasPorInstancia << endl;

        for(auto j = 0; j < cantMaxAristasPorInstancia; j++)
        {
            output << rand() % (cantEquipos + cantAristasPorInstancia) << ' ';
            equipo1 = (rand() % cantEquipos);
            equipo2 = (rand() % cantEquipos);
            while(equipo1 == equipo2 || unidos[equipo1][equipo2])
            {
                equipo1 = (rand() % cantEquipos);
                equipo2 = (rand() % cantEquipos);
            }
            unidos[equipo1][equipo2] = true;
            unidos[equipo2][equipo1] = true;
            output << equipo1+1 << ' ' << equipo2+1 << endl;
        }
    }
    return 0;
}
