#include <utility>
#include <vector>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <time.h>
using namespace std;

//Typedefs
typedef vector<vector<int> > Peligros;
typedef vector<vector<int> > Camiones;
typedef vector<int>			 Productos;
//Clases adicionales
struct Solucion
{
	int cant_camiones;
	vector<int> camion_por_producto;

	Solucion(int c){
		this->cant_camiones = c;
	}
};

//Prototipado de funciones
Solucion resolver(int n, int capacidad, Peligros& peligros_ij);
void backtracking(Productos& productos, Peligros& peligros_ij, int m, Camiones& camiones, int& optima_cant, Camiones& resultado);
bool check_umbral(Camiones& camiones, Peligros& peligros_ij, int m);
void print(Camiones& camiones);
void saveResult(Camiones& camiones, Camiones& resultado);
int buscarProd(int prod, Camiones& resultado);
int peligrosidad(int prod_i, int prod_j, Peligros& peligros_ij);
int dar_cota_inicial(Productos& productos, Peligros& peligros_ij, int m);


int main()
{
	int repeticiones = 750;	//Las repeticiones es para sacar un promedio, ya que el CPU atiende otros procesos ademas de este.
  	double secs;
	clock_t t_ini, t_fin;
	ofstream averagesFile ("promediosEj3.txt");
	averagesFile << "Para cada instancia cargada, se realizaron " << repeticiones << " repeticiones de ella para sacar el promedio.\nTambién se muestra el mejor tiempo obetenido de esas repeticiones.\n" << endl;

	int n;
	int capacidad;
	cin >> n;
	int contadorDeInstancias = 0;
	double bestTimesAcum = 0;
	while(n != 0){
		contadorDeInstancias++;
		int repetir = repeticiones;	//Esta es la variable que se va decrementando en el whle de repeticiones.
		double partial = 0;

		//Letura del input
		cin >> capacidad;
		Peligros peligros_ij;
		for(int i = 1; i < n; i++){
			vector<int> peligros_i(n-i, 0);
			for(int j = 0; j < n-i; j++){
				cin >> peligros_i[j];
			}
			peligros_ij.push_back(peligros_i);
		}

		double menorTiempo = -1;
		while(repetir > 0){

			t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo

			Solucion solucion = resolver(n, capacidad, peligros_ij);

			t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.
			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".
			partial += secs;

			if(secs < menorTiempo || menorTiempo == -1){
	    		menorTiempo = (double)secs;
	    	}

			repetir--;
		}
		bestTimesAcum+= menorTiempo;

		cout << "\nMejor tiempo: " << 1000.0 * menorTiempo << " milisegundos." << endl;
		cout << "Promedio: " << 1000 * (partial/repeticiones) << " milisegundos." << endl;
    	if(averagesFile.is_open()){
    		averagesFile << "\nMejor tiempo para " << n << " productos con umbral "<< capacidad << ": "  << 1000.0 * menorTiempo << " milisegundos." << endl;
    		averagesFile << "Promedio para " << n << " productos con umbral "<< capacidad << ": "  << 1000 * (partial/repeticiones) << " milisegundos.\n\n";
    	}


		cin >> n;
	}

	averagesFile << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos.";
	cout << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos." << endl;

	cout << "\nSe guardaron los promedios de todas las instancias en un archivo" << endl;
	averagesFile.close();

	return 0;
}


Solucion resolver(int n, int capacidad, Peligros& peligros_ij){
	Camiones camiones;
	Productos productos(n, 0);
	Camiones resultado;

	//int optima_cantidad = n;
	int optima_cantidad = dar_cota_inicial(productos, peligros_ij, capacidad);

	backtracking(productos, peligros_ij, capacidad, camiones, optima_cantidad, resultado);

	Solucion solucion(resultado.size());
	for(int i = 0; i < n; i++){
		solucion.camion_por_producto.push_back(buscarProd(i,resultado));

	}
	
	return solucion;

}

int dar_cota_inicial(Productos& productos, Peligros& peligros_ij, int m){

	int peligrosidadActual = 0;
	int primeroACargar = 0;
	int res = 0;
	int futuraPeligrosidad = 0;
	int cantProductos = productos.size();

	for(int i2 = 0; i2 < cantProductos; i2++){
		for(int j2 = primeroACargar; j2 < i2; j2++){
			futuraPeligrosidad = peligrosidadActual + peligrosidad(i2,j2, peligros_ij);
			bool esLaUltimaPregunta = i2 == cantProductos-1  &&  j2 == i2 -1;
			if(futuraPeligrosidad >= m || esLaUltimaPregunta){
				res++;
				peligrosidadActual = 0;
				primeroACargar = i2;
			}else{
				peligrosidadActual = futuraPeligrosidad;
			}
		}
	}
	return res;
}

int peligrosidad(int prod_i, int prod_j, Peligros& peligros_ij){
	if(prod_i < prod_j){
		return peligros_ij[prod_i][prod_j-prod_i-1];
	} else{
		return peligros_ij[prod_j][prod_i-prod_j-1];
	}
}


void backtracking(Productos& productos, Peligros& peligros_ij, int m, Camiones& camiones, int& optima_cant, Camiones& resultado){
	//si se cumple que todos los camiones cumplen con el umbral y si no se están generando más camiones que una conf anterior.
	if(camiones.size() <= optima_cant && check_umbral(camiones, peligros_ij, m)){ 
		if(productos.size() < 1){ //cuando ya se agregaron todos los productos.
			optima_cant = camiones.size();
			saveResult(camiones, resultado);
			return;
		}

		int ultimo = productos.size() - 1;
		productos.pop_back();
		for(int i = 0; i < camiones.size(); i++){
			camiones[i].push_back(ultimo); //pongo un producto en el camion i
			backtracking(productos, peligros_ij, m, camiones, optima_cant, resultado);
			camiones[i].pop_back(); //cuando vuelvo del backtrack, deshago el cambio
		}

		vector<int> nuevo_camion; 
		nuevo_camion.push_back(ultimo); //este es para agregar el producto en un camion individual
		camiones.push_back(nuevo_camion);
		backtracking(productos, peligros_ij, m, camiones, optima_cant, resultado);
	} else {
		return;
	}
}

bool check_umbral(Camiones& camiones, Peligros& peligros_ij, int m){
	for(int i = 0; i < camiones.size(); i++){
	int acum = 0;
		if(camiones[i].size() > 1){	//Camiones con 1 solo elemento no tienen peligrosidad...
			for(int j = 0; j < camiones[i].size()-1; j++){
				for(int k = j+1; k < camiones[i].size(); k++){
					int p2 = camiones[i][j];
					int p1 = camiones[i][k];
					acum = acum + peligros_ij[p1][p2-p1-1];
				}
				if(acum >= m){
					return false;
				}
			}
		}
	}	
	return true;
}

void print(Camiones& camiones){
	for(int i = 0; i < camiones.size(); i++){
		cout << "[ ";
		for(int j = 0; j < camiones[i].size(); j++){
			cout << camiones[i][j] << " " ;
		}
		cout << "] ";
	}
	cout << endl;
	return;
}

void saveResult(Camiones& camiones, Camiones& resultado){
	resultado.clear();
	for(int i = 0; i < camiones.size(); i++){
		vector<int> nuevo;
	    for(int j = 0; j < camiones[i].size(); j++){
	    	nuevo.push_back(camiones[i][j]);
	    }   
	    resultado.push_back(nuevo);
	}
	return;
}

int buscarProd(int prod, Camiones& resultado){
	for(int i = 0; i < resultado.size(); i++){
		for(int j = 0; j < resultado[i].size(); j++){
	    		if(prod == resultado[i][j]){
				return i+1;			
			}
	    }  	
	}
	return 0;
}