#include <iostream> // cout/cin
#include <vector>   // vector
#include <utility>  // pair
#include <queue>    // queue
#include <list>     // list

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

tuplaSolucion prim(const grafo& input, int& costoTotal)
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

            costoTotal += conexionMinFueraAGM.first;

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

camino dameCamino(const predecesores& c, const nodo& inicio, const nodo& fin)
{
    camino camino;
    nodo recorredor = fin;

    while(recorredor != inicio)
    {
        camino.push_back(recorredor);
        recorredor = c[recorredor];
    }
    camino.push_back(inicio);

    return camino;
}

grafo tomarInput()
{
    auto nodos = 0;
    cin >> nodos;
    auto conexiones = 0;
    cin >> conexiones;
    auto nodo1 = 0;
    auto nodo2 = 0;
    auto costo = 0;
    grafo input(nodos,vector<int>(nodos, -1));

    for(auto i = 0; i < conexiones; i++)
    {
        cin >> costo;
        cin >> nodo1;
        cin >> nodo2;
        nodo1 -= 1;
        nodo2 -= 1;
        unir(input, nodo1, nodo2, costo);
    }

    return input;
}

void darOutput(const grafo& input, grafo& AGMInput, const nodo& nodo1, const nodo& nodo2, int& costoTotal)
{
    predecesores preds = BFSMod(AGMInput, nodo1, nodo2);
    camino caminoADevolver = dameCamino(preds, nodo1, nodo2);

    auto cantConexionesServidor = caminoADevolver.size();
    auto cantEquipos = AGMInput.size();

    camino::const_iterator it1 = caminoADevolver.begin();
    camino::const_iterator it2 = caminoADevolver.begin();
    it2++;

    vector<vector<bool>> printeados(cantEquipos, vector<bool>(cantEquipos,false));

    cout << costoTotal << ' ' << cantConexionesServidor << ' ' << cantEquipos - cantConexionesServidor << endl;

    while ( it2 != caminoADevolver.end() )
    {
        cout << AGMInput[*it1][*it2] << ' ' << (*it1)+1 << ' ' << (*it2)+1 << endl;
        printeados[*(it1)][*(it2)] = true;
        printeados[*(it2)][*(it1)] = true;
        it1++;
        it2++;
    }

    printeados[nodo2][nodo1] = true;
    printeados[nodo1][nodo2] = true;
    cout << input[nodo1][nodo2] << ' ' << nodo1+1 << ' ' << nodo2+1 << endl;

    for(auto i = 1; i < cantEquipos; ++i)
    {
        for(auto j = 0; j < i; ++j)
        {
            if(!(printeados[i][j]) && AGMInput[i][j] >= 0)
            {
                cout << AGMInput[i][j] << ' ' << i+1 << ' ' << j+1 << endl;
                printeados[i][j] = true;
            }
        }
    }
}

int main(int argc, char** argv)
{
    grafo input = tomarInput();

    int costoTotal = 0;
    tuplaSolucion AGMInput = prim(input, costoTotal);

    tuplaCiclo ciclo = encontrarConexionMinParaAnillo(input, AGMInput.first);
    parNodo formaCiclo = ciclo.first;
    bool hayCiclo = ciclo.second;
    cout << endl;

    if( AGMInput.second && hayCiclo )
    {
        costoTotal += input[formaCiclo.first][formaCiclo.second];
        darOutput(input, AGMInput.first, formaCiclo.first, formaCiclo.second, costoTotal);
    }
    else
    {
        cout << "no";
    }
    return 0;
}
