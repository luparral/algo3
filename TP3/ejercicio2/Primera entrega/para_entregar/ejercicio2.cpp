#include <utility>
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

//Typedefs
typedef vector<int>			 Nodos;
typedef vector<int> 		 Solucion;
typedef vector<int>			 Conjunto;
typedef vector<Conjunto > 	 Particion;
typedef vector<vector<int> > Pesos;



//Prototipado de funciones
int pesoDeConjunto(Conjunto& conjunto, Pesos& pesos);
void resolver(int n, Pesos& pesos, Nodos& nodos, int k, Solucion& solucion);
void backtracking(Nodos& nodos, Pesos& pesos, Particion& particion, int& optima_cant, Particion& resultado, int k);
int pesoDeParticion(Particion& particion, Pesos& pesos);
void saveResult(Particion& particion, Particion& resultado);
int buscarNodo(int prod, Particion& resultado);
int pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos);
void swap(int x, int y, Nodos& nodos);
int generarParticionInicial(Nodos& nodos, Pesos& pesos, int k, Particion& resultado);

int main(){
	//Tomo input
	int n, m, k;
	cin >> n >> m >> k;
	
	//Almaceno los nodos
	Nodos nodos(n,0);
	for (int i = 0; i < n; i++)	nodos[i] = i;

	//Almaceno los pesos de cada par de nodos (pesos de aristas)
	Pesos pesos_ij;
	pesos_ij.reserve(n);
	for(int i = 1; i < n; i++){
		vector<int> pesos_de_arista_i(n-i, 0);
		pesos_ij.push_back(pesos_de_arista_i);
	}

	for (int i = 0; i < m; i++){
		int x, y, p;
		cin >> x >> y >> p;
		
		if(x < y)
			pesos_ij[x-1][y-x-1] = p;
		else
			pesos_ij[y-1][x-y-1] = p;
	}

	//Con los datos almacenados en estructuras, llamo a la resolucion del problema
	Solucion solucion;
	solucion.reserve(n);
	resolver(n, pesos_ij, nodos, k, solucion);

	//Imprimo resultado en pantalla
	for(int i = 0; i < n; i++)	cout << solucion[i] << " ";
	cout << endl;
	return 0;
}

void resolver(int n, Pesos& pesos, Nodos& nodos, int k, Solucion& solucion){
	Particion particion;
	Particion resultado(k,Conjunto());

	//Hago una primer cota para mejorar los tiempos.
	int optima_cantidad = generarParticionInicial(nodos, pesos, k, resultado);

	//Realizo el backtracking
	backtracking(nodos, pesos, particion, optima_cantidad, resultado, k);

	//Armo y devuelvo la solucion
	for(int i = 0; i < n; i++)
		solucion.push_back(buscarNodo(i,resultado));

	return;
}

void backtracking(Nodos& nodos, Pesos& pesos, Particion& particion, int& optima_cant, Particion& resultado, int k){
	int pesoParticion = pesoDeParticion(particion, pesos);
	if(particion.size() <= k && pesoParticion < optima_cant){
		if(nodos.empty()){
            optima_cant = pesoParticion;
            saveResult(particion, resultado);
			return;
		}

		for (int j = nodos.size()-1; j >= 0; j--){
		if(nodos.size()-1 != j)
			swap(nodos.size()-1, j, nodos);

		int ultimo = nodos.back();
		nodos.pop_back();
		for(int i = 0; i < particion.size(); i++){
			particion[i].push_back(ultimo); 
			backtracking(nodos, pesos, particion, optima_cant, resultado, k);
			particion[i].pop_back();
		}

		Conjunto nuevo_conjunto;
		nuevo_conjunto.push_back(ultimo);
		particion.push_back(nuevo_conjunto);

		backtracking(nodos, pesos, particion, optima_cant, resultado, k);

		particion.pop_back();
		nodos.push_back(ultimo);
		
		if(nodos.size()-1 != j)
			swap(nodos.size()-1, j, nodos);
		}

	}

	return;
}

int generarParticionInicial(Nodos& nodos, Pesos& pesos, int k, Particion& resultado){
	int set = 0;
	bool aumentar = true;
	for(int nodo = 0; nodo < nodos.size(); nodo++){
		if(aumentar)
			resultado.push_back(Conjunto());
		
		resultado[set].push_back(nodo);

		set++;
		if(set == k){
			set = 0;
			aumentar = false;
		}
	}

	return pesoDeParticion(resultado, pesos);
}

void swap(int x,int y,Nodos& nodos){
	int temp = nodos[x];
	nodos[x] = nodos[y];
	nodos[y] = temp;
}

int pesoDeParticion(Particion& particion, Pesos& pesos){
	int acum = 0;
	for (int i = 0; i < particion.size(); i++)
		acum += pesoDeConjunto(particion[i],pesos);

	return acum;
}

int pesoDeConjunto(Conjunto& conjDeNodos, Pesos& pesos){
	if(conjDeNodos.size() < 2)
		return 0;

	int acum = 0;
	for(int i = 0; i < conjDeNodos.size()-1; i++)
		for(int j = i+1; j < conjDeNodos.size(); j++)
			acum += pesoDeArista(conjDeNodos[i], conjDeNodos[j], pesos);

	return acum;
}

int pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos){
	if(nodo_i < nodo_j)
		return pesos[nodo_i][nodo_j-nodo_i-1];
	else
		return pesos[nodo_j][nodo_i-nodo_j-1];
}

void saveResult(Particion& particion, Particion& resultado){
	resultado.clear();
	resultado.reserve(particion.size());

	for(int i = 0; i < particion.size(); i++){
		Conjunto nuevo;
		nuevo.reserve(particion[i].size());

	    for(int j = 0; j < particion[i].size(); j++)
	    	nuevo.push_back(particion[i][j]);

	    resultado.push_back(nuevo);
	}
	return;
}

int buscarNodo(int nodo, Particion& resultado){
	for(int i = 0; i < resultado.size(); i++)
		for(int j = 0; j < resultado[i].size(); j++)
	    	if(nodo == resultado[i][j])
				return i+1;
	return 0;
}
