#include <vector>
#include <iostream>
#include <utility>	//std::pair
#include <math.h>   //floor
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
	int cant_edificios;

	cout << "\nBienvenido al programa Horizontes Lejanos!\n" << endl;

	cout << "Ingrese la cantidad de edificios a resolver:";
	cin >> cant_edificios;
	cout << "\n";
	while(cant_edificios != 0){

		//cada edificio tiene 3 variables (x1, y, x2)
		Edificio edificio(3, 0);
		Edificios edificios(cant_edificios, edificio);

		for (int i = 0; i < cant_edificios; ++i){
			cin >> edificios[i][0]; //x1
			cin >> edificios[i][1]; //y
			cin >> edificios[i][2]; //x2
		}

		//Se convierten edificios a horizontes
		Horizontes horizontes = EdificiosAHorizontes(edificios);

		Horizonte resultado = UnirHorizontes(horizontes, 0, horizontes.size()-1);

		for (int i = 0; i < resultado.size(); ++i){
			cout << resultado[i].first << " " << resultado[i].second << " ";
		}

		cout << "\nIngrese la cantidad de edificios a resolver:\n";
		cin >> cant_edificios;
	}

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
		//(x1, y, x2) => (x1, y)(x2, 0)
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
	Horizonte resultado;
	resultado.reserve( h1.size() + h2.size() );
	//Ultima coordenada accedida de cada horizonte (mas legibilidad de codigo)
	Coordenada ulth1;
	Coordenada ulth2;
	//indice i y j que recorren el horizonte 1 y 2 respectivamente.
	int i = 0;
	int j = 0;

	//INVARIANTE: hasta la posicion i+j del resultado, este contiene el sub-contorno final
	while( i < h1.size() || j < h2.size() ){

		//si ya se recorrio todo el horizonte
		//entonces agrego todas las coordenadas del que faltan
		if( i >= h1.size() ){
			resultado.push_back(h2[j]);
			j++;
		} else 
		if( j >= h2.size() ){
			resultado.push_back(h1[i]);
			i++;
		} else
		//caso actualh1.x < actualh2.x	
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
		//caso actualh1.x > actualh2.x, analogo
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
		//caso actualh1.x == actualh2.x
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