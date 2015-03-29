#include <iostream> // cout/cin
#include <vector>   // vector
#include <utility>  // pair
#include <queue>    // queue
#include <list>     // list
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <limits>
#include <iomanip>

typedef std::numeric_limits< double > dbl;

using namespace std;

typedef int nodo;
typedef int peso;
typedef vector<vector<peso> > grafo;
typedef pair<peso, nodo> pesoPredecesor;
typedef vector<pesoPredecesor> vectorPesoPredecesor;
typedef pair<nodo, nodo> parNodo;
typedef vector<nodo> predecesores;
typedef pair<grafo, bool> tuplaSolucion;
typedef pair<parNodo, bool> tuplaCiclo;
typedef list<nodo> camino;


inline void unir(grafo& g, const nodo& nodo1, const nodo& nodo2, const int& val)
{
    if( nodo1 < g.size() && nodo2 < g.size() )
    {
        g[nodo1][nodo2] = val;
        g[nodo2][nodo1] = val;
    }
}

tuplaSolucion prim(const grafo& input)
{
    auto cantNodos = input.size();

    grafo res(cantNodos, vector<peso>(cantNodos,-1));

    vectorPesoPredecesor diccionario(cantNodos, make_pair(-1,-1));

    vector<bool> unidos(cantNodos, false);
    unidos[0] = true;

    auto cn = 0;
    nodo nodoActual = 0;
    auto sigo = true;

    pesoPredecesor conexionMinFueraAGM;
    nodo nodoAConectar;

    while(cn < cantNodos && sigo)
    {
        for(auto i = 0; i < cantNodos; i++)
        {
            if((!(unidos[i])) && (input[nodoActual][i] >= 0) &&
               ((diccionario[i].first == - 1) || (input[nodoActual][i] < diccionario[i].first)))
            {
                diccionario[i] = make_pair(input[nodoActual][i], nodoActual);
            }
        }

        conexionMinFueraAGM = diccionario[0];
        nodoAConectar = 0;
        for(auto i = 0; i < cantNodos; i++)
        {
            if((!(unidos[i])) && diccionario[i].first >= 0)
            {
                if(conexionMinFueraAGM.first == -1 || diccionario[i].first < conexionMinFueraAGM.first)
                {
                    conexionMinFueraAGM = diccionario[i];
                    nodoAConectar = i;
                }
            }
        }

        if(nodoAConectar != 0)
        {
            unir(res, nodoAConectar, conexionMinFueraAGM.second, conexionMinFueraAGM.first);
            unidos[nodoAConectar] = true;
            nodoActual = nodoAConectar;
            cn++;
        }
        else
        {
            sigo = false;
            cn++;
        }
    }

    return make_pair(res, (sigo) || (cn == cantNodos));
}

tuplaCiclo encontrarConexionMinParaAnillo(const grafo& input, const grafo& AGMInput)
{
    parNodo conexionCostoMin = make_pair(0, 0);
    auto n = input.size();
    bool existe = false;
    for(auto i = 1; i < n; i++)
    {
        for(auto j = 0; j < i; j++)
        {
            if( (AGMInput[i][j] == -1) && (input[i][j] > 0) )
            {
                if(input[conexionCostoMin.first][conexionCostoMin.second] == -1)
                {
                    conexionCostoMin = make_pair(i,j);
                    existe = true;
                }
                else if(input[i][j] < input[conexionCostoMin.first][conexionCostoMin.second])
                {
                    conexionCostoMin = make_pair(i,j);
                    existe = true;
                }
            }
        }
    }

    return make_pair(conexionCostoMin, existe);
}

predecesores BFSMod(const grafo& AGM, const nodo& inicio, const nodo& destino)
{
    auto cantNodos = AGM.size();

    predecesores predecesores(cantNodos,-1);
    vector<bool> visitados(cantNodos,false);
    visitados[inicio] = true;

    queue<nodo> cola;
    cola.push(inicio);

    nodo nodoActual = inicio;

    while(!(cola.empty()) && nodoActual != destino)
    {
        cola.pop();

        for(nodo i = 0; i < cantNodos; i++)
        {
            if(!visitados[i] && AGM[nodoActual][i] >= 0)
            {
                predecesores[i] = nodoActual;
                visitados[i] = 1;
                cola.push(i);
            }
        }

        nodoActual = cola.front();
    }

    return predecesores;
}

grafo tomarInput(ifstream& finput)
{
    auto nodos = 0;
    finput >> nodos;
    auto conexiones = 0;
    finput >> conexiones;
    auto nodo1 = 0;
    auto nodo2 = 0;
    auto costo = 0;
    grafo input(nodos,vector<int>(nodos, -1));

    for(auto i = 0; i < conexiones; i++)
    {
        finput >> costo;
        finput >> nodo1;
        finput >> nodo2;
        nodo1 -= 1;
        nodo2 -= 1;
        unir(input, nodo1, nodo2, costo);
    }

    return input;
}

void printGrafo(grafo& g)
{
    auto n = g.size();
    for(auto i = 0; i < n; i++)
    {
        cout << '[';
        for(auto j = 0; j < n; j++)
        {
           if(g[i][j] < 0)
           {
               cout << g[i][j];
           }
           else
           {
               cout << '+' << g[i][j];
           }
           if(j < n-1) cout << ' ';
        }
        cout << ']' << endl;
    }
}

int main(int argc, char** argv)
{
    ifstream finput("C:/Emanuel/Facu/Algo 3/Git/AlgoIII/TP2/ejercicio_3/InstanciasRandom.txt");
    clock_t t_ini, t_fin;
    unsigned int repesRestantes;
    unsigned int instanciaActual = 0;
    double mejorTiempo = -1;
    double secs;
    double parcial = 0;
    double mejorTiempoReal;
    double promedioReal;

    while(instanciaActual < 1)
    {
        mejorTiempo = -1;
        secs = 0.0;
        parcial = 0.0;
        promedioReal = 0.0;
        mejorTiempoReal = 0.0;
        cout << "Tomo input instancia " << instanciaActual+1 << endl;
        grafo input = tomarInput(finput);
        cout << "Cantidad de equipos " << input.size() << endl;
        repesRestantes = 500;
        while(repesRestantes > 0)
        {
            t_ini = clock();
            tuplaSolucion AGMInput = prim(input);
            tuplaCiclo ciclo = encontrarConexionMinParaAnillo(input, AGMInput.first);
            t_fin = clock();
            secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
            if(secs < mejorTiempo || mejorTiempo == -1){
                mejorTiempo = (double)secs;
            }
            parcial += secs;
            repesRestantes--;
        }
        mejorTiempoReal = (mejorTiempo * 1000.0) / 2.0;
        cout << std::setprecision(dbl::digits10) << "El mejor tiempo para instancia " << instanciaActual+1 << " fue: " << mejorTiempoReal << endl;
        instanciaActual++;
    }
    return 0;
}
