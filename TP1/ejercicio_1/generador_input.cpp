#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


int main(){
	ofstream myfile ("inputRandomEJ1.txt");
	cout << "\nBienvenido al generador de instancias aleatorias del juego de los puentes sobre lava caliente\n\n";
	
	cout << "Elija la opcion que desee, en cuanto a tiempo de computo." << endl;
	cout << "	1. [Mejor caso] El participante puede cruzar todo el puente de 1 solo salto." << endl;
	cout << "	2. [Caso aleatorio] Seleccion de cantidad de tablones y capacidad de salto." << endl;
	cout << "	3. [Peor caso] El participante puede dar a lo sumo 1 salto y puede cruzar todo el puente" << endl;
	int caseTest;
	cin >> caseTest;

	cout << "\nIngresa cuantas instancias deseas generar: ";
	int cant;
	cin >> cant;
	
	if(myfile.is_open()){
		int instance_number = 0;
		srand ( time(NULL) );
		while(cant > 0){
			instance_number++;
			int n;
			cout << "\n	Cantidad de tablones de la instancia " << instance_number << ": ";
			cin >> n;

			int m = n+1;
			if(caseTest == 2){
				cout << "	Salto maximo de la instancia " << instance_number << ": ";
				cin >> m;
			}else if(caseTest == 3){
				m = 1;
			}

			int i = 0;

			myfile << n;
			myfile << " ";
			myfile << m;
			myfile << " ";
			while(i < n){
				int r = rand()%2;
				if(caseTest == 3){
					r = 0;
				}
				myfile << r;
				myfile << " ";
				i++;
			}
			cant--;
		}
			
	}
	myfile << "0\n";	
	myfile.close();

	cout << "\nSe genero correctamente el archivo fuente inputRandomEJ1.txt\n";

	return 0;
}
