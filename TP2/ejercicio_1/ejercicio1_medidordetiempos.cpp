#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm> 
#include <limits>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <time.h>

using namespace std;

//Clases
struct Vuelo{
	int origen;
	int destino;
	int inicio;
	int fin;
	int ID;

	Vuelo(){
		this->origen = 0;
		this->destino = 0;
		this->inicio = 0;
		this->fin = 0;
		this->ID = -1;
	}

	Vuelo(int o, int d, int i, int f, int ID){
		this->origen = o;
		this->destino = d;
		this->inicio = i;
		this->fin = f;
		this->ID = ID;
	}
};

struct Cuatrupla{
	string origen;
	string destino;
	int despegue;
	int llegada;
};

struct Itinerario{
	Vuelo siguienteVuelo;
	bool llego;
	bool calculado;
	int tarda;
	int arranca;
	int cantViajes;

	Itinerario(){
		this->siguienteVuelo = Vuelo();
		this->llego = false;
		this->calculado = false;
		this->tarda = numeric_limits<int>::max();
		this->arranca = 0;
		this->cantViajes = numeric_limits<int>::max();
	}

	Itinerario(Vuelo flights, bool puedeLlegar, bool calcule, int tiempo, int cuandoArranca, int cantVuelos){
		this->siguienteVuelo = flights;
		this->llego = puedeLlegar;
		this->calculado = calcule;
		this->tarda = tiempo;
		this->arranca = cuandoArranca;
		this->cantViajes = cantVuelos;
	}

};

int binarySearch(int n, vector<Vuelo>& vuelos);
Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& mapaVuelosDeSalida, vector< Itinerario >& best, vector<bool>& disponibles);
bool orden(Vuelo a, Vuelo b);

int main(){
	int repeticiones = 100;
	double secs;
	clock_t t_ini, t_fin;
	ofstream averagesFile;
	averagesFile.open("promediosEj1.txt", std::ios::app);
	//averagesFile << "Para cada instancia cargada, se realizaron " << repeticiones << " repeticiones de ella para sacar el promedio.\nTambién se muestra el mejor tiempo obetenido de esas repeticiones.\n" << endl;
 
	//double partial = 0;
	double menorTiempo = -1;
	bool hay_solucion = false;
	
	map<string, int> mappingCityToInt;
	//lectura de parametros iniciales
	string A;
	string B;
	int n;

	cin >> A;
	cin >> B;

	cin >> n;
	
	Cuatrupla temp[n];

	mappingCityToInt[A] = 0;
	mappingCityToInt[B] = 1;

	vector< vector<Vuelo> > mapaVuelosDeSalida(2,vector<Vuelo>());
	mapaVuelosDeSalida.reserve(2*n + 2);


	
	//prepara el arreglo con los datos
	for (int cargador = 0; cargador < n; cargador++){
		string o;
		string d;
		int inicio;
		int f;

		cin >> o;
		cin >> d;
		cin >> inicio;
		cin >> f;

		temp[cargador].origen = o;
		temp[cargador].destino = d;
		temp[cargador].despegue = inicio;
		temp[cargador].llegada = f;
	}

	while(repeticiones > 0){//tantas veces como repeticiones quiero hacer


		t_ini = clock();
		//recorre el arreglo con los datos y los mapea al mapa -para tomar en cada repeticion la complejidad del mapeo-	
		for (int j = 0; j < n; j++){

			string o;
			string d;
			int inicio;
			int f;

			o = temp[j].origen;
			d = temp[j].destino;
			inicio = temp[j].despegue;
			f = temp[j].llegada;

			if(mappingCityToInt.count(o) == 0){
				int firstDisponiblePositionOfMap = mappingCityToInt.size();

				mapaVuelosDeSalida.push_back(vector<Vuelo>());
				mapaVuelosDeSalida.back().reserve(n);

				mappingCityToInt[o] = firstDisponiblePositionOfMap;
			}

			if(mappingCityToInt.count(d) == 0){
				int firstDisponiblePositionOfMap = mappingCityToInt.size();

				mapaVuelosDeSalida.push_back(vector<Vuelo>());
				mapaVuelosDeSalida.back().reserve(n);
				
				mappingCityToInt[d] = firstDisponiblePositionOfMap;
			}
			
			int intDeCiudadOrigen = mappingCityToInt[o];
			int intDeCiudadDestino = mappingCityToInt[d];

			Vuelo nuevoVueloConstruido = Vuelo(intDeCiudadOrigen, intDeCiudadDestino, inicio, f, j); 
			mapaVuelosDeSalida[intDeCiudadOrigen].push_back(nuevoVueloConstruido);
		}

		//mapeo del input
		for(int j = 0; j < mapaVuelosDeSalida.size(); j++){	//SORTING EN LAS mapaVuelosDeSalida
			vector<Vuelo>& flightsToSort = mapaVuelosDeSalida[j];
			stable_sort(flightsToSort.begin(), flightsToSort.end(),orden);
		}

		vector<bool> disponibles(mappingCityToInt.size(),true);
		vector< Itinerario > best(mappingCityToInt.size(),Itinerario());


		Itinerario result = bestWay(0,0,mapaVuelosDeSalida,best, disponibles);

		t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.

		if(!result.llego){
			//cout << "no" << endl;
		}else{
			//cout << best[0].tarda << " ";
			//cout << best[0].cantViajes << " ";

			Vuelo aux = best[0].siguienteVuelo;
			//cout << aux.ID << " ";
			while(aux.destino != 1){
				aux = best[aux.destino].siguienteVuelo;
				//cout << aux.ID << " ";
			}
			//cout << endl;
		}

		secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".

		if(secs < menorTiempo || menorTiempo == -1){
	    		menorTiempo = (double)secs;
	    }

		repeticiones--;
	}

	cout << "\nMejor tiempo: " << 1000.0 * menorTiempo << " milisegundos." << endl;
    if(averagesFile.is_open()){
    	averagesFile << 1000.0 * menorTiempo << endl;
    }
    averagesFile.close();
	return 0;
}



//Funciones
Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& mapaVuelosDeSalida, vector< Itinerario >& best, vector<bool>& disponibles){
	int hasta = numeric_limits<int>::max();

	if(best[origen].calculado){	//Si estoy mirando desde un origen que ya fue calculado

		if(best[origen].arranca == hora){	// Y lo fue a la misma hora en la que estoy
			return best[origen];			//simplemente lo devuelvo
		}

		if(best[origen].arranca < hora){//Si no, si la hora del calculo era menor
			if(!best[origen].llego){
				//Si la hora de calculo era menor, y ya no había podido llegar hasta B, entonces mas tarde tampoco podrá
				return best[origen];	//Por lo que devuelvo el valor guardado, que dice que no se puede.
			}else{
				//Si la hora de calculo era menor, pero SI pudo llegar, entonces, se que hay otro camino mas rápido
				//Y no me interesa seguir viendo este
				return Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);
			}
		}else{
			hasta = best[origen].arranca;
		}

	}

	if(origen == 1){	
		//Si el origen es B, es un caso base, entonces, guardo en B un itinerario que dice que pudo llegar
			// y que la mejor forma de llegar a B desde B fue calculada.
		best[origen] = Itinerario(Vuelo(), true, true, hora, hora, 0);
		return best[origen]; //Y la devuelvo
	}

	//Si llegé hasta esa parte es porque no estoy en B,
	//o porque no hay un calculo hecho o el calculo hecho no me sirvió

	Itinerario itinerario = Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);

	bool noEsNecesarioSeguirRevisando = false;
	int t = 0;
	if(mapaVuelosDeSalida[origen].size() > 0)
		t = binarySearch(hora, mapaVuelosDeSalida[origen]);

	for(t; t < mapaVuelosDeSalida[origen].size(); t++){
		Vuelo vuelo = mapaVuelosDeSalida[origen][t];
		Itinerario recursive;

		if(best[origen].calculado && hasta + 2 <= vuelo.inicio){
				recursive = best[origen];
				noEsNecesarioSeguirRevisando = true;

				if(recursive.llego && recursive.tarda < itinerario.tarda){
					itinerario = Itinerario(vuelo,true,true,recursive.tarda,hora,1 + recursive.cantViajes);
				}
				
				break;
		}


		if(vuelo.inicio >= hora + 2 && disponibles[vuelo.destino]){
				disponibles[origen] = false;
				recursive = bestWay(vuelo.destino, vuelo.fin, mapaVuelosDeSalida, best, disponibles);
				disponibles[origen] = true;
				

				if(recursive.llego && recursive.tarda < itinerario.tarda){
					itinerario = Itinerario(vuelo,true,true,recursive.tarda,hora,1 + recursive.cantViajes);
				}
		}
		
	};

	//A esta altura del código, la variable "itinerario", despeus de revisar todos los vuelso de salida, si se
	//puede llegar a B, contiene el que en menor tiempo lo hace. Sino, se guarda indicando que no se puede llegar.

	best[origen] = itinerario;

	return best[origen];
}

bool orden(Vuelo a, Vuelo b){
    if(a.inicio == b.inicio){
        return a.fin <= b.fin;
    }else{
        return a.inicio < b.inicio;
    }
}

int binarySearch(int n, vector<Vuelo>& vuelos){
	int beg = 1;
	int end = vuelos.size();

	int mid = (beg+end)/2;
	if(vuelos[end-1].inicio < n)
		return end;

	if(vuelos[0].inicio > n)
		return 0;

	while(beg<=end && vuelos[mid].inicio !=n ){
		if(vuelos[mid].inicio < n)
			beg=mid+1;
		else
			end=mid-1;
		mid=(beg+end)/2;
	}

	return mid + 1;
}