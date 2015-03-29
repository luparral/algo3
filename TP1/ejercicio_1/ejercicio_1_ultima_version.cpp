#include <utility>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

//Typedefs
typedef vector<int> Puente;

/* 
Special characters replacements:
á por \240 
é por \202 
í por \241 
ó por \242 
ú por \243 
*/


int main(){
	//Comienza lectura de input
	cout << "\nBienvenido al juego de puentes sobre lava caliente" << endl;

	int salto_maximo;
	int cant_tablones;
	cout << "\nColoc\240 cuantos tablones quer\202s: ";
	cin >> cant_tablones;
	while(cant_tablones > 0){
		cout << "\nColoc\240 el salto m\240ximo: ";
		cin >> salto_maximo;
		Puente puente(cant_tablones, 0);
		cout << "\nColoca el estado de los tablones, (0 es sano y 1 es roto)\n";
		for(int i = 0; i < cant_tablones; i++){
			cout << "Tabl\242n " << i + 1 << ": ";
			cin >> puente[i];
		}
		//Finalizada la lectura, comienza el algoritmo:

		vector<int> saltos;				
		//Saltos es el vector que almacena los sucesivos saltos que da el jugador
		saltos.reserve(cant_tablones + 2);
		bool hay_solucion = false;		
		//Variable que indica si se podrá cruzar el puente.
		int actual = 0;					
		//"Actual" representa el tablón actual en el que el jugador está posicionado.
		int longest_posible = 0;		
		//longuest_posible va almacenando el tablón sano mas lejano al que podemos saltar.

    	if(salto_maximo > cant_tablones){ 
    	//¿Recorro todo el puente de un solo salto?
			saltos.push_back(cant_tablones + 1);
			hay_solucion = true;
		}else if(salto_maximo < 1){		  
		//¿La persona puede saltar al menos 1? De no poder hacerlo, termina el juego.
			hay_solucion = false;
		}else{
			//Si no puede cruzarlo de una, pero puede saltar, revisaremos si puede llegar hasta el final, anotando saltos.
			for(int i = 0; i < cant_tablones; i++){
				int tablon_en_revision = i + 1;					
				//Transformación de indice ya que los tablones se numeran desde 1
				
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
				}else if(tablon_en_revision - actual == salto_maximo){	
				//Sino, ¿al menos termine de revisar el rango posible?
					//Si lo hice, salto al tablon longuest_posible que es quien tiene el salto más lejano almacenado.
					actual = longest_posible;
					saltos.push_back(longest_posible);
				}

				//¿Estoy tan cerca del final como para dar un salto y salir del puente?
				if(actual + salto_maximo > cant_tablones){		
					hay_solucion = true;				
					//Si lo estaba, pude cruzar el puente completamente
					saltos.push_back(cant_tablones+1);	
					//Guardo un salto hasta justo despues del último tablón.
					break;								
					//Termino la revision de tablones.
				}
		    }
		}
    	
    	//De la revisión de los tablones y los sucesivos saltos, ¿pude cruzar el puente?
		if(hay_solucion){
			cout << saltos.size() << endl;	
			//Imprimo la cantidad de saltos
			//Si pude cruzar el puente, imprimo los saltos que hice.
			for(int i = 0; i < saltos.size(); i++){
    			cout << saltos[i] << endl;
    		}
		}else{
			//Sino, imprimo que no.
			cout << "no";
		}

		//Preparo para leer la siguiente instancia.
    	cout << "Cargame otra instancia, colocando de nuevo cuantos tablones quer\202s: ";
    	cin >> cant_tablones;
		//Si la cantidad de tablones es < 1, se invalida la guarda del "while", terminado el programa.
	}

	return 0;
}		
