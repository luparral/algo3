#include <vector>
#include <iostream>
#include <utility>	//std::pair
#include <math.h>   //floor
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <time.h>
using namespace std;

//Una coordenada es una tupla de pares de ejes X e Y
//Cada horizonte es un vector de coordenadas
//Horizontes es un vector que tiene muchos horizonte adentro
typedef pair<int, int> Coordenada;
typedef vector<Coordenada> Horizonte;
typedef vector<Horizonte> Horizontes;

typedef vector<int> Edificio;
typedef vector<Edificio> Edificios;

Horizontes EdificiosAHorizontes(const Edificios& edificios);
Horizonte UnirHorizontes(const Horizontes& horizontes, int inicio, int fin);
Horizonte Unir(const Horizonte& h1, const Horizonte& h2);

int main(){
	/*
	ATENCION: Este medidor, acepta instancias del problema y las prueba la cantidad de veces que indica
	la variable "repeticiones" y a partir de eso devuelve un promedio de tiempo de ejecucion de cada instancia.
	Si se carga mas de 1 instancia, calculará los promedios de todas por separado.
	También para cada instancia, almacena el mejor tiempo de todas las repeticiones que haga (marcado por la variable "repeticiones").
	Ademas, el programa calcula el PROMEDIO de todos los MEJORES TIEMPOS, es decir, se toman los mejores tiempos de cada repeticion de instancia y se promedian,
	esto es muy util cuando son entradas de mismo tamaño pero de distinto contenido.
	Si se desea ver efectivamente todos los promedios y sus mejores tiempos, este programa los guarda por separado en el archivo llamado "promedios_ej2.txt".
	*/

  	//Variables para medición de tiempo.
	int repeticiones = 500;	//Las repeticiones es para sacar un promedio, ya que el CPU atiende otros procesos ademas de este.
  	double secs;
	clock_t t_ini, t_fin;
	ofstream averagesFile ("promedios_ej2.txt");
	averagesFile << "Para cada instancia cargada, se realizaron " << repeticiones << " repeticiones de ella para sacar el promedio.\nTambién se muestra el mejor tiempo obetenido de esas repeticiones.\n" << endl;

	//Flujo normal del programa, eliminando escrituras en consola que no son pertinentes a los fines de ESTE programa medidor.
	int cant_edificios;
	cin >> cant_edificios;
	int contadorDeInstancias = 0;
	double bestTimesAcum = 0;

	while(cant_edificios != 0){
		contadorDeInstancias++;
		int repetir = repeticiones;	//Esta es la variable que se va decrementando en el whle de repeticiones.
		double partial = 0;

		//cada edificio tiene 3 variables (x1, y, x2)
		Edificio edificio(3, 0);
		Edificios edificios(cant_edificios, edificio);

		for (int i = 0; i < cant_edificios; ++i){
			cin >> edificios[i][0]; //x1
			cin >> edificios[i][1]; //y
			cin >> edificios[i][2]; //x2
		}

		/*
		*
		*Inicio medición
		*
		*/
		double menorTiempo = -1;
		while(repetir > 0){

			t_ini = clock(); // Tomo el tiempo antes de ejecutar el algoritmo

			Horizontes horizontes = EdificiosAHorizontes(edificios); //Se convierten edificios a horizontes
			Horizonte resultado = UnirHorizontes(horizontes, 0, horizontes.size()-1);	//Ejecuto algoritmo

			t_fin = clock(); // Tomo el tiempo luego de ejecutar el algoritmo.
			
			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; // Hago la resta de t_fin y t_ini y la guardo como double en "secs".
	    	partial += secs;								 // Aumento la suma parcial

			if(secs < menorTiempo || menorTiempo == -1){
	    		menorTiempo = (double)secs;
	    	}

			repetir--;
		}
		bestTimesAcum+= menorTiempo;

		cout << "\nMejor tiempo: " << 1000.0 * menorTiempo << " milisegundos." << endl;
		cout << "Promedio: " << 1000 * (partial/repeticiones) << " milisegundos." << endl;
    	if(averagesFile.is_open()){
    		averagesFile << "\nMejor tiempo: " << cant_edificios << " cant_edificios: "  << 1000.0 * menorTiempo << " milisegundos." << endl;
    		averagesFile << "Promedio para " << cant_edificios << " cant_edificios: " << 1000 * (partial/repeticiones) << " milisegundos.\n\n";
    	}

		cin >> cant_edificios;

	}

	averagesFile << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos.";
	cout << "\nEl PROMEDIO de los MEJORES TIEMPOS de las " << contadorDeInstancias << " instancias es: "<< 1000.0 * (bestTimesAcum/contadorDeInstancias) <<" milisegundos." << endl;

	cout << "\nSe guardaron los promedios de todas las instancias en un archivo" << endl;
	averagesFile.close();

	return 0;
}

Horizontes EdificiosAHorizontes(const Edificios& edificios){
	//inicializo las coordenadas en x=0 e y=0
	//asigno a cada horizonte dos coordenadas, porque al principio son edificios
	//tengo ahora N horizontes de 2 coordenadas cada uno
	int cant_edificios = edificios.size();
	Coordenada coord;
	Horizonte horizonte(2, coord);
	Horizontes horizontes(cant_edificios, horizonte);

	for (int i = 0; i < cant_edificios; ++i){
		//elijo el horizonte i, y le asigno valores a la primera y segunda coordenada
		horizontes[i][0].first = edificios[i][0];
		horizontes[i][0].second = edificios[i][1];
		horizontes[i][1].first = edificios[i][2];
		horizontes[i][1].second = 0;
	}

	return horizontes;
}

//el UnirHorizontes es recursivo y va dividiendo el vector de horizontes en problemas mas chicos 
//caso base: es un solo horizonte o son dos, y los une, es un estilo de merge sort
//caso recursivo: el vector de horizontes tiene mas de dos horizontes, entonces se llama recursivamente con la mitad de cada uno
Horizonte UnirHorizontes(const Horizontes& horizontes, int inicio, int fin){
	if(fin - inicio == 0){
		return horizontes[inicio];
	} else
	if(fin - inicio == 1){
		return Unir(horizontes[inicio], horizontes[fin]);
	} else 
	if(fin - inicio > 1) {
		int mitad = floor((fin - inicio)/2) + inicio;
		return Unir( UnirHorizontes(horizontes, inicio, mitad), UnirHorizontes(horizontes, mitad+1, fin));
	}
}

Horizonte Unir(const Horizonte& h1, const Horizonte& h2){
	//Inicializo un horizonte resultado
	//inicializo variables para mantener la ultima coordenada accedida de cada horizonte (mas legibilidad de codigo)
	//inicializo indice i y j para recorrer el horizonte 1 y 2 respectivamente.
	Horizonte resultado;
	resultado.reserve( h1.size() + h2.size() );
	Coordenada ulth1;
	Coordenada ulth2;
	int i = 0;
	int j = 0;

	//la clave es ir detectando los cambios de altura
	//busco siempre al primero que aparece en el eje x, sea h1 o h2.

	///PROPIEDADES
	//una vez que lo tengo a la coordenada que primero aparece, hay 3 casos a analizar respecto a la ultima coordenada del horizonte opuesto:
	//	1-que sea mayor, 2-que sea menor, 3-que sea igual (respecto del eje X)
	//	el caso que sea mayor/menor son analogos (con variables invertidas)
	//INVARIANTE: hasta la posicion i+j del resultado, este contiene contorno formado hasta ese momento!!
	while( i < h1.size() || j < h2.size() ){

		//si alguno de los indices, alcanzo su maximo, significa que ya no tiene mas coordenadas ese horizonte y termino
		//entonces agrego todas las coordenadas del que faltan
		if( i >= h1.size() ){
			resultado.push_back(h2[j]);
			j++;
		} else 
		if( j >= h2.size() ){
			resultado.push_back(h1[i]);
			i++;
		} else
		//caso h1.x < h2.x	
		if(h1[i].first < h2[j].first){
			//si es mayor en altura que la ultima posicion del h2, entonces ya gane
			if(h1[i].second > ulth2.second){
				resultado.push_back(h1[i]);
			}
			//si es menor en altura que la ultima posicion del h2, entonces puede pasar que sean:
			//esta coordenada esta contenida entre dos coordenadas de h2, deberia ignorarla
			//el horizonte h2 sufre una caida y el h1 lo sobrepasa, deberia actualizar el eje x con la nueva altura
			//si tienen la misma altura entonces lo ignoro
			if(h1[i].second <= ulth2.second){
				if(ulth1.second > ulth2.second){
					Coordenada coord(h1[i].first, ulth2.second);
					resultado.push_back(coord);
				}
			}
			if(h1[i].second == 0 && ulth2.second == 0){
				resultado.push_back(h1[i]);
			}
			ulth1 = h1[i];
			i++;
		} else
		//caso h1.x > h2.x, analogo
		if(h1[i].first > h2[j].first){
			if(h2[j].second > ulth1.second){
				resultado.push_back(h2[j]);
			}
			if(h2[j].second < ulth1.second){
				if(ulth2.second > ulth1.second){
					Coordenada coord(h2[j].first, ulth1.second);
					resultado.push_back(coord);
				}
			}
			if(h2[j].second == 0 && ulth1.second == 0){
				resultado.push_back(h2[j]);
			}
			ulth2 = h2[j];
			j++;
		} else
		//caso h1.x == h2.x
		//ambas coordenadas son las mismas en el eje x, agrego solo la mas alta, salteo la mas chica
		if(h1[i].first == h2[j].first){
			if(h1[i].second >= h2[j].second){
				resultado.push_back(h1[i]);
				
			} else {
				resultado.push_back(h2[j]);
				
			}
			ulth1 = h1[i];
			ulth2 = h2[j];
			i++;
			j++;
		}
	}

	return resultado;
}