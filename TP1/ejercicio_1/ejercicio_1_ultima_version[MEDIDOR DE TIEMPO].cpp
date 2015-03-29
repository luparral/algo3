#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <time.h>
using namespace std;

//Typedefs
typedef vector<int> Puente;

int main(){
	/*
	ATENCION: Este medidor, acepta instancias del problema y las prueba la cantidad de veces que indica
	la variable "repeticiones" y a partir de eso devuelve un promedio de tiempo de ejecucion de cada instancia.
	Si se carga mas de 1 instancia, calculará los promedios de todas por separado.
	También para cada instancia, almacena el mejor tiempo de todas las repeticiones que haga (marcado por la variable "repeticiones").
	Ademas, el programa calcula el PROMEDIO de todos los MEJORES TIEMPOS, es decir, se toman los mejores tiempos de cada repeticion de instancia y se promedian,
	esto es muy util cuando son entradas de mismo tamaño pero de distinto contenido.
	Si se desea ver efectivamente todos los promedios y sus mejores tiempos, este programa los guarda por separado en el archivo llamado "promedios_ej1.txt".
	*/

  	//Variables para medición de tiempo.
	int repeticiones = 500;	//Las repeticiones es para sacar un promedio, ya que el CPU atiende otros procesos ademas de este.
  	double secs;
	clock_t t_ini, t_fin;
	ofstream averagesFile ("promedios_ej1.txt");
	averagesFile << "Para cada instancia cargada, se realizaron " << repeticiones << " repeticiones de ella para sacar el promedio.\nTambién se muestra el mejor tiempo obetenido de esas repeticiones.\n" << endl;
	
	int salto_maximo;
	int cant_tablones;
	cin >> cant_tablones;
	int contadorDeInstancias = 0;
	double bestTimesAcum = 0;
	bool hay_solucion = false;
	while(cant_tablones > 0){
		contadorDeInstancias++;
		int repetir = repeticiones;	//Esta es la variable que se va decrementando en el whle de repeticiones.
		double partial = 0;

		cin >> salto_maximo;
		Puente puente(cant_tablones, 0);
		for(int i = 0; i < cant_tablones; i++){
			cin >> puente[i];
		}
		double menorTiempo = -1;
		while(repetir > 0){
			t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo

			vector<int> saltos;				//Saltos es el vector que almacena los sucesivos saltos que da el jugador
			saltos.reserve(cant_tablones + 2);
			hay_solucion = false;		//Variable que indica si se podrá cruzar el puente.
			int actual = 0;					//"Actual" representa el tablón actual en el que el jugador está posicionado.
			int longest_posible = 0;		//longuest_posible va almacenando el tablón sano mas lejano al que podemos saltar.

	    	if(salto_maximo > cant_tablones){ //¿Recorro todo el puente de un solo salto?
				saltos.push_back(cant_tablones + 1);
				hay_solucion = true;
			}else if(salto_maximo < 1){		  //¿La persona puede saltar al menos 1? De no poder hacerlo, termina el juego.
				hay_solucion = false;
			}else{
				//Si no puede cruzarlo de una, pero puede saltar, revisaremos si puede llegar hasta el final, anotando saltos.
				for(int i = 0; i < cant_tablones; i++){
					int tablon_en_revision = i + 1;					//Transformación de indice ya que los tablones se numeran desde 1
					
					//La posición actual está sano?
					if(puente[i] == 0){								
						//Si lo es, la guardo como la mas lejana posible a la cual podría saltar
						longest_posible = tablon_en_revision;		
					}

					//¿Terminé de revisar el rango de salto y no puedo saltar a ninguno?
					if(tablon_en_revision - actual == salto_maximo && longest_posible == actual){
						//Entonces no puedo cruzarlo
						hay_solucion = false;
			            break;
					}else if(tablon_en_revision - actual == salto_maximo){	//Sino, ¿al menos termine de revisar el rango posible?
						//Si lo hice, salto al tablon longuest_posible que es quien tiene el salto más lejano almacenado.
						actual = longest_posible;
						saltos.push_back(longest_posible);
					}

					//¿Estoy tan cerca del final como para dar un salto y salir del puente?
					if(actual + salto_maximo > cant_tablones){		
						hay_solucion = true;				//Si lo estaba, pude cruzar el puente completamente
						saltos.push_back(cant_tablones+1);	//Guardo un salto hasta justo despues del último tablón.
						break;								//Termino la revision de tablones.
					}
			    }
			}

	    	t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.

	    	secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".
	    	partial += secs;								 // Aumento la suma parcial

	    	if(secs < menorTiempo || menorTiempo == -1){
	    		menorTiempo = (double)secs;
	    	}
			
			repetir--;
		}

		bestTimesAcum+= menorTiempo;

		if(hay_solucion){
			cout << "\n[Tablones: "<< cant_tablones <<"] [Salto: "<< salto_maximo << "] Mejor tiempo: " << 1000.0 * menorTiempo << " milisegundos y pudo cruzar." << endl;
			cout << "[Tablones: "<< cant_tablones <<"] [Salto: "<< salto_maximo << "] Promedio: " << 1000.0 * (partial/repeticiones) << " milisegundos y pudo cruzar." << endl;
	    	if(averagesFile.is_open()){
	    		averagesFile << "Mejor tiempo para "<< cant_tablones <<" tablones y salto de " << salto_maximo << ": " << 1000.0 * menorTiempo << " milisegundos y pudo cruzar." << endl;
	    		averagesFile << "Promedio para " << cant_tablones << " tablones y salto de " << salto_maximo << ": " << 1000.0 * (partial/repeticiones) << " milisegundos y pudo cruzar.\n\n";
	    	}
	    }else{
	    	cout << "\n[Tablones: "<< cant_tablones <<"] [Salto: "<< salto_maximo << "] Mejor tiempo: " << 1000.0 * menorTiempo << " milisegundos y NO pudo cruzar." << endl;
	    	cout << "[Tablones: "<< cant_tablones <<"] [Salto: "<< salto_maximo << "] Promedio: " << 1000.0 * (partial/repeticiones) << " milisegundos y NO pudo cruzar." << endl;
	    	if(averagesFile.is_open()){
	    		averagesFile << "Mejor tiempo para "<< cant_tablones <<" tablones y salto de " << salto_maximo << ": " << 1000.0 * menorTiempo << " milisegundos y NO pudo cruzar." << endl;
	    		averagesFile << "Promedio para " << cant_tablones << " tablones y salto de " << salto_maximo << ": " << 1000.0 * (partial/repeticiones) << " milisegundos y NO pudo cruzar.\n\n";
	    	}
	    }

    	cin >> cant_tablones;
	}

	averagesFile << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos.";
	cout << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos." << endl;
	cout << "\nSe guardaron los promedios de todas las instancias en un archivo" << endl;
	averagesFile.close();
	return 0;
}		
