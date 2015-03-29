#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm> 
#include <limits>
#include <list>

using namespace std;

//Structs vuelo e itinerario.
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

	Itinerario(Vuelo flight, bool puedeLlegar, bool calcule, int tiempo, int cuandoArranca, int cantVuelos){
		this->siguienteVuelo = flight;
		this->llego = puedeLlegar;
		this->calculado = calcule;
		this->tarda = tiempo;
		this->arranca = cuandoArranca;
		this->cantViajes = cantVuelos;
	}

};

//Definición de funciones
int customBinarySearch(int n, vector<Vuelo>& vuelos);
void cargarDatosDeVuelos(map<string, int>& mappingCityToInt, vector< vector<Vuelo> >& mapaVuelosDeSalida, int n);
void ordenarVuelosDeSalida(vector< vector<Vuelo> >& mapaVuelosDeSalida);
Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& mapaVuelosDeSalida, vector< Itinerario >& best,vector<bool>& disponibles);
void printResult(Itinerario& result, vector< Itinerario >& best);
bool orden(Vuelo a, Vuelo b);


int main(){
		map<string, int> mappingCityToInt;	//Variable que almacenará las distintas ciudades con su respectivo ID numérico
		string A, B;
		int n;		//Variable que almacenará la cantidad de vuelos

		cin >> A;
		cin >> B;
		cin >> n;
		
		mappingCityToInt[A] = 0;	//Mapeo a A con el int 0
		mappingCityToInt[B] = 1;	//Mapeo a B con el int 1

		vector< vector<Vuelo> > mapaVuelosDeSalida(2,vector<Vuelo>());	//Matriz que contendrá los vuelos de salida por ciudad
		mapaVuelosDeSalida.reserve(2*n + 2);	//A lo sumo habrá (2*n + 2) ciudades distintas, así que reservo memoria
		
		cargarDatosDeVuelos(mappingCityToInt, mapaVuelosDeSalida, n);
		ordenarVuelosDeSalida(mapaVuelosDeSalida);

		//////////////////////////////////////////////
		//Calculo de mejor camino/////////////////////
		//////////////////////////////////////////////
		vector<bool> disponibles(mappingCityToInt.size(),true); //Vector que representa la disponibilidad de una ciudad
		vector<Itinerario> best(mappingCityToInt.size(),Itinerario());//Inicializo con itinerarios sin calcular.
		Itinerario result = bestWay(0,0,mapaVuelosDeSalida,best,disponibles);

		//////////////////////////////////////////////
		//Salida por consola de la solución obtenida//
		//////////////////////////////////////////////
		printResult(result, best);

		return 0;
}


Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& mapaVuelosDeSalida, vector< Itinerario >& best, vector<bool>& disponibles){
	int hasta = numeric_limits<int>::max();
	bool noEsNecesarioSeguirRevisando = false;
	int t = 0;
	Itinerario itinerario = Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);

	if(best[origen].calculado){	//Si estoy mirando desde un origen que ya fue calculado

			if(best[origen].arranca == hora)	// Y lo fue a la misma hora en la que estoy
					return best[origen];
			

			if(best[origen].arranca < hora){//Si no, si la hora del calculo era menor
					if(!best[origen].llego)
							return best[origen];
					else
							return Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);
			}else{
					hasta = best[origen].arranca;
			}

	}

	if(origen == 1){	//Si el origen es B (ID de ciudad B es 1)
			best[origen] = Itinerario(Vuelo(), true, true, hora, hora, 0);
			return best[origen];
	}

	//Si llegé hasta esa parte es porque no estoy en B,
	//o porque no hay un calculo hecho o el calculo hecho no me sirvió

	if(mapaVuelosDeSalida[origen].size() > 0)
			t = customBinarySearch(hora, mapaVuelosDeSalida[origen]);

	for(t; t < mapaVuelosDeSalida[origen].size(); t++){	//Reviso los vuelos que salen
			Vuelo vuelo = mapaVuelosDeSalida[origen][t];				//Tomo un vuelo
			if(vuelo.inicio >= hora + 2 && disponibles[vuelo.destino]){	//Si es un vuelo valido (sale en horario válido y no intenta ciclar)
					Itinerario recursive;
					
					if(best[origen].calculado && hasta + 2 <= vuelo.inicio){
							recursive = best[origen];
							noEsNecesarioSeguirRevisando = true;
					}else{
							disponibles[origen] = false;	//Marco a la ciudad como no disponible para evitar calculos de vuelos que vuelvan hacia aca
							recursive = bestWay(vuelo.destino, vuelo.fin, mapaVuelosDeSalida, best, disponibles);
							disponibles[origen] = true;		//Ahora que volví de la recursion, vuelvo a marcar a la ciudad como disponible
					}

					if(recursive.llego && recursive.tarda < itinerario.tarda)
							itinerario = Itinerario(vuelo,true,true,recursive.tarda,hora,1 + recursive.cantViajes);

					if(noEsNecesarioSeguirRevisando)
							break;
			}
		
	};

	//A esta altura del código, la variable "itinerario", despeus de revisar todos los vuelso de salida, si se
	//puede llegar a B, contiene el que en menor tiempo lo hace. Sino, se guarda indicando que no se puede llegar.

	best[origen] = itinerario;

	return best[origen];
}


int customBinarySearch(int n, vector<Vuelo>& vuelos){
	/*
	 * Es una búsqueda custom, pues para los casos donde el n es menor al horario de salida del primer vuelo
	 * o es mayor al ultimo, lo devuelve en O(1). Mientras que si n está entre el primero y el ultimo vuelo, 
	 * es O(log(vuelos.size());
	 */

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

void printResult(Itinerario& result, vector< Itinerario >& best){
	if(!result.llego){
			cout << "no" << endl;
	}else{
			cout << best[0].tarda << " ";
			cout << best[0].cantViajes << " ";

			Vuelo aux = best[0].siguienteVuelo;
			cout << aux.ID << " ";

			while(aux.destino != 1){
					aux = best[aux.destino].siguienteVuelo;
					cout << aux.ID << " ";
			}

			cout << endl;
	}

	return;
}


///////////////////////////////////////////////////////////////////////////
//Funciones para manejo de datos, usadas en la primer parte del programa///
///////////////////////////////////////////////////////////////////////////

bool orden(Vuelo a, Vuelo b){
    if(a.inicio == b.inicio)
        	return a.fin <= b.fin;
    else
        	return a.inicio < b.inicio;
}

void cargarDatosDeVuelos(map<string, int>& mappingCityToInt, vector< vector<Vuelo> >& mapaVuelosDeSalida, int n){
		//En esta función se tomaran los datos de los n vuelos del problema.
		//Nos interesa saber la cantidad de ciudades distintas, por lo que iremos guardando las nuevas que aparezcan, en un mapa
		//y así sabremos si una ciudad ya había aparecido o no, consultando si está definida en el mapa.

		for (int j = 0; j < n; j++){
			string o, d;	//Vars que contendrán el origen y el destino del vuelo j
			int inicio, f;	//Vars que contendrá la hora de salida y llegada del vuelo j

			cin >> o;
			cin >> d;
			cin >> inicio;
			cin >> f;

			if(mappingCityToInt.count(o) == 0){	//Si la ciudad de origen no había aparecido antes
					int firstDisponiblePositionOfMap = mappingCityToInt.size();

					mapaVuelosDeSalida.push_back(vector<Vuelo>());	//Agrando un lugar el mapa que tendrá vuelos de salida
					mapaVuelosDeSalida.back().reserve(n);

					mappingCityToInt[o] = firstDisponiblePositionOfMap;	//la anoto como ciudad nueva con el próximo ID
			}

			if(mappingCityToInt.count(d) == 0){	//Si la ciudad de destino no había aparecido antes
					int firstDisponiblePositionOfMap = mappingCityToInt.size();

					mapaVuelosDeSalida.push_back(vector<Vuelo>());	//Agrando un lugar el mapa que tendrá vuelos de salida
					mapaVuelosDeSalida.back().reserve(n);
					
					mappingCityToInt[d] = firstDisponiblePositionOfMap;	//la anoto como ciudad nueva con el próximo ID
			}
			
			
			int intDeCiudadOrigen = mappingCityToInt[o];
			int intDeCiudadDestino = mappingCityToInt[d];

			//Construyo el struct vuelo
			Vuelo nuevoVueloConstruido = Vuelo(intDeCiudadOrigen, intDeCiudadDestino, inicio, f, j+1); 
			
			//A la ciudad origen del vuelo, le agrego el vuelo como vuelo de salida
			mapaVuelosDeSalida[intDeCiudadOrigen].push_back(nuevoVueloConstruido);
		}

		return;
}

void ordenarVuelosDeSalida(vector< vector<Vuelo> >& mapaVuelosDeSalida){
	//Para cada ciudad distinta, ordeno todos los vuelos de salida por horario de llegada, DE MAYOR A MENOR
	//Ya que el for que los recorra mas tarde, recorre de atras para adelante.
	for(int j = 0; j < mapaVuelosDeSalida.size(); j++){
			vector<Vuelo>& flightsToSort = mapaVuelosDeSalida[j];
			stable_sort(flightsToSort.begin(), flightsToSort.end(),orden);
	}

	return;
}
