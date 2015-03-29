#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <list>
#include <limits>
#include "trie/diccString.h"
using namespace std;

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
Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& salidas, vector< Itinerario >& best, vector<bool>& disponibles);
bool orden(Vuelo a, Vuelo b);
void printTodo(vector< Itinerario >& best);
void printVuelo(Vuelo& vuelo);
//void printVuelos(list<Vuelo>& vuelos);

int main(){
	DiccString<Nat> mapping = DiccString<Nat>();
	string A;
	string B;
	int n;

	cout << "Dame A: ";
	cin >> A;
	cout << "\nDame B: ";
	cin >> B;

	cout << "\nDame la cantidad de vuelos: ";
	cin >> n;
	cout << "\n" << endl;
	
	mapping.Definir(A,0);
	mapping.Definir(B,1);

	//PREGUNTAR SI HAY PROBLEMA EN RESERVAR MEMORIA LUEGO DE ASIGNAR 2 POSICIONES
	vector< vector<Vuelo> > salidas(2,vector<Vuelo>());
	salidas.reserve(2*n + 2);

	for (int j = 0; j < n; j++){
		string o;
		string d;
		int inicio;
		int f;

		cout << "Origen de vuelo " << j << ": ";
		cin >> o;
		cout << "\nDestino de vuelo " << j << ": ";
		cin >> d;
		cout << "\nInicio de vuelo " << j << ": ";
		cin >> inicio;
		cout << "\nFin de vuelo " << j << ": ";
		cin >> f;
		cout << "\n" << endl;

		if(!mapping.Definido(o)){
			salidas.push_back(vector<Vuelo>());

			mapping.Definir(o,mapping.CantClaves());
		}
		
		if(!mapping.Definido(d)){
			salidas.push_back(vector<Vuelo>());
			mapping.Definir(d,mapping.CantClaves());
		}
		
		int intDeCiudadOrigen = mapping.Significado(o);
		int intDeCiudadDestino = mapping.Significado(d);

		Vuelo nuevoVueloConstruido = Vuelo(intDeCiudadOrigen, intDeCiudadDestino, inicio, f, j); 
		salidas[intDeCiudadOrigen].push_back(nuevoVueloConstruido);
	}
	
	for(int j = 0; j < salidas.size(); j++){	//SORTING EN LAS SALIDAS
		vector<Vuelo>& flightToSort = salidas[j];
		stable_sort(flightToSort.begin(), flightToSort.end(),orden);
	}

	vector< Itinerario > best(mapping.CantClaves(),Itinerario());
	vector<bool> disponibles(mapping.CantClaves(), true);	

	Itinerario result = bestWay(0,0,salidas,best,disponibles);
	printTodo(best);
	if(!result.llego){
		cout << "no" << endl;
	}else{
		cout << best[0].tarda << " ";
		cout << best[0].cantViajes << " ";

		Vuelo aux = best[0].siguienteVuelo;
		cout << aux.ID << " ";
		// cout << "En " << 0 << " tomar el que va a " << aux.destino;
		while(aux.destino != 1){
			aux = best[aux.destino].siguienteVuelo;
			cout << aux.ID << " ";
			// cout << "En " << aux.origen << " tomar el que va a " << aux.destino << "\n\n";
		}
		cout << endl;
	}

	return 0;
}

Itinerario bestWay(int origen, int hora, vector< vector<Vuelo> >& salidas, vector< Itinerario >& best, vector<bool>& disponibles){
	cout << "\n//////////////////////////////Revisando desde la ciudad " << origen << " hasta B a las " << hora << "hs" << endl;
	if(best[origen].calculado){
		cout << "estaba calculado entre la ciudad " << origen << " hasta B a las " << hora << "hs" << endl;
		if(best[origen].arranca == hora){
			return best[origen];
		}

		if(!best[origen].llego){
			if(best[origen].arranca < hora){
				return best[origen];
			}
		}else{
			if(best[origen].arranca < hora){
				return Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);
			}
		}
	}

	if(origen == 1){
		best[origen] = Itinerario(Vuelo(), true, true, hora, hora, 0);
		return best[origen];
	}

	// if(!salidas[origen].size()){
	// 	best[origen] = Itinerario(list<Vuelo>(), false, true, numeric_limits<int>::max(), hora);
	// 	return best[origen];
	// }

	//vector<Vuelo> outFlights = salidas[origen];

	Itinerario itinerario = Itinerario(Vuelo(), false, true, numeric_limits<int>::max(), hora, 0);
	for(int t = salidas[origen].size() -1; t >= 0; t--){
	//while(outFlights.size() > 0){
		Vuelo vuelo = salidas[origen][t];

		// salidas[origen].pop_back();
		//salidas[origen].erase(salidas[origen].begin()+ t);

		cout << "	Prueba con el ";
		printVuelo(vuelo);

		if(vuelo.inicio >= hora + 2 && disponibles[vuelo.destino]){
			cout << "\nle gusto ya que la hora es " << hora << endl;
			disponibles[origen] = false;
			Itinerario recursive = bestWay(vuelo.destino, vuelo.fin, salidas, best, disponibles);
			disponibles[origen] = true;
			//recursive.vuelos.push_front(vuelo);

			/*if(itinerario.tarda == -1){
				itinerario = Itinerario(recursive.vuelos,recursive.llego,true,recursive.vuelos.back().fin,hora);
			}*/
			if(recursive.llego){
				cout << "\n/////////////////Revisandoooooo " << origen << " hasta B a las " << hora << "hs" << endl;
				//cout << "resursive.vuelos.back.fin: " << recursive.vuelos.back().fin << endl;
				cout << "tardaaaaaaaaaaaaaaaaaaaaaa " << recursive.tarda << endl;
			}

			if(recursive.llego && recursive.tarda < itinerario.tarda){
				itinerario = Itinerario(vuelo,true,true,recursive.tarda,hora, 1 + recursive.cantViajes);
			}

			/*if(itinerario.llego){
				if(!best[origen].calculado){
					best[origen] = itinerario;
				}else{
					if(itinerario.tarda < best[origen].tarda){
						best[origen] = itinerario;
					}
				}
			}*/
		}else{
			cout << "\nNO le gusto ya que la hora es " << hora << endl;
		}
		//salidas[origen].push_back(vuelo);
	};

	/*if(!best[origen].calculado){
		best[origen] = Itinerario(list<Vuelo>(), false, true, 0);
	}*/
	best[origen] = itinerario;

	return best[origen];
}

bool orden(Vuelo a, Vuelo b){
    if(a.inicio == b.inicio){
        return a.fin >= b.fin;
    }else{
        return a.inicio > b.inicio;
    }
}

void printTodo(vector< Itinerario >& best){
	cout << "[THE PROGRAM HAS ENDED] Desde cualquier ciudad, el mejor camino hasta B es: \n" << endl;
	for(int i = 0; i < best.size(); i++){
		cout << "	Desde " << i << ":  ";
		if(best[i].calculado){
			if(i == 1)
				cout << "La ciudad numero 1 es B" << endl;
			else if(best[i].siguienteVuelo.ID == -1)
				cout << "[No es posible llegar hasta B porque no vuelos que lo logren o desde aquí no es la mejor manera]" << endl;
			else
				printVuelo(best[i].siguienteVuelo);
		}else{
			cout << "[No calculado, ya que desde ciudades anteriores no se podía llegar aca]";
		}
		cout << endl;
	}
	cout << endl;
	
	return;
}

void printVuelo(Vuelo& v){
	cout << "Vuelo" << v.ID <<": ["<< v.origen << " - " << v.destino << " (de " << v.inicio << " a " << v.fin << ")]\n                  ";
	return;
}

/*void printVuelos(list<Vuelo>& vuelos){
	if(vuelos.size() == 0)
		cout << "[No es posible llegar hasta B, no hay vuelos]";
	else
		for (list<Vuelo>::iterator it=vuelos.begin(); it != vuelos.end(); ++it)
    		printVuelo(*it);
 	return;
}*/

int binarySearch(int n, vector<Vuelo>& vuelos){
	int beg = 1;
	int end = vuelos.size();

	int mid = (beg+end)/2;                

	while(beg<=end && vuelos[mid].inicio !=n ){      // Compare Item and Value of Mid
		if(vuelos[mid].inicio < n)
			beg=mid+1;
		else
			end=mid-1;
		mid=(beg+end)/2;
	}

	if(vuelos[mid].inicio == n){
		return mid;
	}
	else {
		return mid + 1;
	}
}