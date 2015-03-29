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
void resolver(int n, Pesos& pesos, int k, Solucion& solucion);
void backtracking(Nodos& nodos, Pesos& pesos, Particion& particion, int& optima_cant, Particion& resultado, int k);
int pesoDeParticion(Particion& particion, Pesos& pesos);
int buscarNodo(int prod, Particion& resultado);
int pesoDeArista(int nodo_i, int nodo_j, Pesos& pesos);
int generarParticionInicial(int n, Pesos& pesos, int k);
vector<Particion> getPartitions(Pesos& pesos, int optimo_peso, int n, int k, int originalN);
void addEmptySetWithN(vector<Particion>& preParticiones,vector<Particion>& ret, int k, int n);
void addNIntoExistinConjs(vector<Particion>& preParticiones,vector<Particion>& ret, Pesos& pesos, int& optimo_peso, int n, int originalN);
void print(Particion& particion);
void printPartitions(vector<Particion>& myParticions,Pesos& pesos);

int main(){
	//Tomo input
	int n, m, k;
	cin >> n >> m >> k;

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
	if (k < n)
		resolver(n, pesos_ij, k, solucion);
	else
		for(int i = 1; i <= n; i++)	solucion.push_back(i);

	//Imprimo resultado en pantalla
	for(int i = 0; i < n; i++)	cout << solucion[i] << " ";
	cout << endl;
	return 0;
}

void resolver(int n, Pesos& pesos, int k, Solucion& solucion){
	Particion resultado;
	int optimo_peso = std::numeric_limits<int>::max();

	vector<Particion> partitions = getPartitions(pesos, optimo_peso, n, k, n);

	for (int i = 0; i < partitions.size(); ++i){
		if(pesoDeParticion(partitions[i],pesos) < optimo_peso){
			resultado = partitions[i];
			optimo_peso = pesoDeParticion(partitions[i],pesos);
		}
	}

	//Armo y devuelvo la solucion
	for(int i = 1; i <= n; i++)
		solucion.push_back(buscarNodo(i,resultado));

	return;
}

vector<Particion> getPartitions(Pesos& pesos, int optimo_peso, int n, int k, int originalN){
	if(n == 1)	//Caso base, particion de 1. Devuelve [ 1 ]
		return vector<Particion>(1, Particion(1,Conjunto(1,1)));

	//Caso recursivo.
	vector<Particion> partitionsToReturn;
	vector<Particion> preParticiones = getPartitions(pesos, optimo_peso, n-1, k, originalN);

	addEmptySetWithN(preParticiones, partitionsToReturn, k, n);
	addNIntoExistinConjs(preParticiones, partitionsToReturn, pesos, optimo_peso, n, originalN);
	
	return partitionsToReturn;
}

void addEmptySetWithN(vector<Particion>& preParticiones,vector<Particion>& partitionsToReturn, int k, int n){
	Conjunto conjParaAgregar(1,n);

	for (int j = 0; j < preParticiones.size(); ++j){
		Particion a = preParticiones[j];

		if(a.size() < k){
			a.push_back(conjParaAgregar);
			partitionsToReturn.push_back(a);
		}

	}
};

void addNIntoExistinConjs(vector<Particion>& preParticiones,vector<Particion>& ret, Pesos& pesos, int& optimo_peso, int n, int originalN){
	for (int j = 0; j < preParticiones.size(); ++j){
		Particion b = preParticiones[j];
		for (int i = 0; i < b.size(); ++i){
			b[i].push_back(n);
			if(pesoDeParticion(b,pesos) <= optimo_peso){
				ret.push_back(b);
				if(n == originalN)
					optimo_peso = pesoDeParticion(b,pesos);
			}
			b[i].pop_back();			
		}
	}
};

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
	nodo_i--;
	nodo_j--;

	if(nodo_i < nodo_j)
		return pesos[nodo_i][nodo_j-nodo_i-1];
	else
		return pesos[nodo_j][nodo_i-nodo_j-1];
}

void print(Particion& particion){
	for(int i = 0; i < particion.size(); i++){
		cout << "[ ";
		for(int j = 0; j < particion[i].size(); j++){
			cout << particion[i][j] << " " ;
		}
		cout << "] ";
	}
	cout << endl;
	return;
}

int buscarNodo(int nodo, Particion& resultado){
	for(int i = 0; i < resultado.size(); i++)
		for(int j = 0; j < resultado[i].size(); j++)
	    	if(nodo == resultado[i][j])
				return i+1;
	return 0;
}

void printPartitions(vector<Particion>& myParticions, Pesos& pesos){
	for (int i = 0; i < myParticions.size(); ++i)
		print(myParticions[i]);
}

int generarParticionInicial(int n, Pesos& pesos, int k){
	Particion resultado;
	int set = 0;
	bool aumentar = true;
	
	for(int nodo = 1; nodo <= n; nodo++){
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