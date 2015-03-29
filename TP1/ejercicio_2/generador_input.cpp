#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


int main(){
	ofstream myfile ("inputRandomEJ2.txt");
	int cant;
	cout << "\nBienvenido al generador de instancias aleatorias del juego de los horizontes\n\n";
	cout << "Ingresa cuantas instancias aleatorias deseas generar: ";
	cin >> cant;
	
	if(myfile.is_open()){
		int instance_number = 0;
		srand ( time(NULL) );
		while(cant > 0){
			instance_number++;
			int n;
			cout << "\n	Cantidad de edificios de la instancia " << instance_number << ": ";
			cin >> n;

			myfile << n;
			myfile << "\n";
			int i = 0;
			while(i < n){
				int izq = rand()%100;
				int alt = rand()%100;
				int der = izq + rand()%100;
				myfile << izq;
				myfile << " ";
				myfile << alt;
				myfile << " ";
				myfile << der;
				myfile << "\n";
				i++;
			}
			cant--;
		}
			
	}
	myfile << "0\n";	
	myfile.close();

	cout << "\nSe genero correctamente el archivo fuente inputRandomEJ2.txt\n";

	return 0;
}
