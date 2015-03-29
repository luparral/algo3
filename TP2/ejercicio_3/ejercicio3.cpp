#include <iostream> // cout/cin
#include <vector>   // vector
#include <utility>  // pair
#include <fstream>  // istream/ostream/ifstream
#include <list> // list
#include <queue> // queue

using namespace std;

typedef vector<vector<int> > grafo;
typedef int nodo;
typedef pair<int, nodo> distPadre;
typedef vector<distPadre> vectorPesoPredecesor;
typedef pair<nodo, nodo> parNodo;
typedef pair<grafo, bool> tuplaSolucion;
typedef pair<parNodo, bool> tuplaCiclo;
typedef vector<nodo> predecesores;
typedef list<nodo> camino;

///Requiere que AGMInput sea el AGM de input y que tengan tamaño > 0.
tuplaCiclo encontrarConexionMinParaAnillo(grafo& input, grafo& AGMInput)
{
    auto conexionCostoMin = make_pair(0, 0);
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

void printVector(vector<int>& v)
{
    cout << '[';
    int n = v.size();
    for(int i = 0; i < n; i++)
    {
        cout << v[i];
        if(i != n - 1)
        {
            cout << ", ";
        }
    }
    cout << ']' << endl;
}

camino dameCamino(predecesores& c, nodo& inicio, nodo& fin)
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

void printCamino(camino& c)
{
    for(camino::const_iterator it = c.begin(); it != c.end(); it++)
    {
        cout << *it << ' ';
    }
    cout << endl;
}


///Lo voy a utilizar para encontrar el camino más corto entre los extremos de la arista agregada al AGM para armar la red.
predecesores BFSMod(grafo& AGM, nodo& inicio, nodo& destino)
{
    auto cantNodos = AGM.size();

    predecesores predecesores(cantNodos,-1);
    vector<nodo> visitados(cantNodos,0);
    visitados[inicio] = 1;

    queue<nodo> cola;
    cola.push(inicio);

    nodo nodoActual = inicio;

    while(!(cola.empty()) && nodoActual != destino)
    {
        cola.pop();

        for(nodo i = 0; i < cantNodos; i++)
        {
            if(visitados[i] == 0 && AGM[nodoActual][i] >= 0)
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




inline void unir(grafo& g, nodo& nodo1, nodo& nodo2, int& val)
{
    if( nodo1 < g.size() && nodo2 < g.size() )
    {
        g[nodo1][nodo2] = val;
        g[nodo2][nodo1] = val;
    }
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

tuplaSolucion prim(grafo& input, int& costoTotal)
{
    auto cantNodos = input.size();

    grafo res(cantNodos, vector<int>(cantNodos, -1));

    //A este vector voy a consultar y/o actualizar las conexiones menos pesadas para cada iteración
    vectorPesoPredecesor diccionario(cantNodos, make_pair(-1,0));

    vector<nodo> unidos(cantNodos, 0); //Vector para ver en O(1) si ya fueron colocados en el árbol
    unidos[0] = 1;

    //Variables principales, cn es el contador de pasos del prim que hice, a nodoActual le voy a pedir adyacentes y
    //el bool me va a indicar si se puede armar un AGM a partir del grafo input.
    auto cn = 0;
    nodo nodoActual = 0;
    auto sigo = true;

    //Variables auxiliares
    distPadre conexionMinFueraAGM = diccionario[0];

    while(cn < cantNodos && sigo)
    {
        //Actualizo el vector
        for(auto i = 0; i < cantNodos; i++)
        {
            if((unidos[i] == 0) && (input[nodoActual][i] >= 0) &&
               ((diccionario[i].first == - 1) || (input[nodoActual][i] < diccionario[i].first)))
            {
                diccionario[i] = make_pair(input[nodoActual][i], nodoActual);
            }
        }

        //Busco la mínima conexión a algún nodo no conectado dentro del vector
        conexionMinFueraAGM = diccionario[0];
        nodo nodoAConectar = 0;
        for(auto i = 0; i < cantNodos; i++)
        {
            if(unidos[i] == 0 && diccionario[i].first >= 0)
            {
                if(conexionMinFueraAGM.first == -1 || diccionario[i].first < conexionMinFueraAGM.first)
                {
                    conexionMinFueraAGM = diccionario[i];
                    nodoAConectar = i;
                }
            }
        }

        /*Si hay alguna conexión por hacer, la hago, sino niego el booleano
         Tener en cuenta que puede que no haya porque terminé, por eso el || en el resultado*/

        if(conexionMinFueraAGM.first != -1)
        {
            unir(res, nodoAConectar, conexionMinFueraAGM.second, conexionMinFueraAGM.first);
            unidos[nodoAConectar] = 1;
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


grafo tomarInput(string& pathIn)
{
    ifstream inputStream(pathIn.c_str());
    auto nodos = 0;
    inputStream >> nodos;
    auto conexiones = 0;
    inputStream >> conexiones;

    auto nodo1 = 0;
    auto nodo2 = 0;
    auto costo = 0;
    grafo input(nodos,vector<int>(nodos, -1));

    for(auto i = 0; i < conexiones; i++)
    {
        inputStream >> costo;
        inputStream >> nodo1;
        inputStream >> nodo2;
        nodo1 -= 1; //Necesito restar ya que el enunciado pide que se indexen de 1 a N los equipos.
        nodo2 -= 1; //Es muy molesto más adelante
        unir(input, nodo1, nodo2, costo);
    }

    return input;
}

///PRE == Red sea una red solución válida
void darOutput(grafo& input, grafo& AGMInput, nodo& nodo1, nodo& nodo2, int& costoTotal)
{
    predecesores preds = BFSMod(AGMInput, nodo1, nodo2);
    camino ciclo = dameCamino(preds, nodo1, nodo2);

    auto cantConexionesServidor = ciclo.size();
    auto cantEquipos = AGMInput.size();

    camino::const_iterator it1 = ciclo.begin();
    camino::const_iterator it2 = ciclo.begin();
    vector<nodo> printeados(cantEquipos, 0);
    it2++;

    cout << costoTotal << ' ' << cantConexionesServidor << ' ' << cantEquipos - cantConexionesServidor << endl;

    while ( it2 != ciclo.end() )
    {
        cout << AGMInput[*it1][*it2] << ' ' << (*it1)+1 << ' ' << (*it2)+1 << endl;
        it1++;
        it2++;
        printeados[(*it1)] = 1;
    }

    printeados[nodo2] = 1;
    //No están en el AGM, se devuelven aparte.
    cout << input[nodo1][nodo2] << ' ' << nodo1 << ' ' << nodo2 << endl;

    for(auto i = 1; i < cantEquipos; i++)
    {
        for(auto j = 0; j < i; j++)
        {
            if(printeados[i] == 0 && AGMInput[i][j] >= 0)
            {
                cout << AGMInput[i][j] << ' ' << i+1 << ' ' << j+1 << endl;
                printeados[i] == 1;
            }
        }
    }

}

int main(int argc, char** argv)
{
    /*Tomó el input del path, requiere que el formato de entrada sea válido*/
    string pathIn = "C:/Emanuel/Facu/Algo 3/Git/AlgoIII/TP2/ejercicio_3/RedInput.txt";
    grafo input = tomarInput(pathIn);

    /*Aplico algoritmo de prim. La primer componente es el AGM.
                                La segunda es un bool que indica si la red input es válida para resolver el problema.*/
    int costoTotal = 0;
    tuplaSolucion AGMInput = prim(input, costoTotal);

    /*En caso de que se pueda resolver (que haya un AGM subyacente y una arista disponible)
     busco la arista con menos peso que no esté en el AGM y la coloco. Si no, devuelvo "no" */
    tuplaCiclo ciclo = encontrarConexionMinParaAnillo(input, AGMInput.first);

    parNodo formaCiclo = ciclo.first;
    bool hayCiclo = ciclo.second;

    if(AGMInput.second && hayCiclo )
    {
        /*Utilizo la versión modificada del BFS para encontrar el camino entre los 2 nodos que formarán el servidor aprovechando
        las características que tiene un AGM.*/
        costoTotal += input[formaCiclo.first][formaCiclo.second];

        /*Luego, manejo el output*/
        darOutput(input, AGMInput.first, formaCiclo.first, formaCiclo.second, costoTotal);
    }
    else
    {
        cout << "no";
    }

    return 0;
}
