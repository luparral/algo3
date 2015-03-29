#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


int main(){
	ofstream myfile ("inputRandomEJ3.txt");
	cout << "\nBienvenido al generador de instancias aleatorias de Biohazard\n\n";
	
	cout << "\nIngresa cuantas instancias deseas generar: ";
	int cant;
	cin >> cant;
	
	if(myfile.is_open()){
		int instance_number = 0;
		srand ( time(NULL) );
		while(cant > 0){
			instance_number++;
			int n;
			cout << "\n   Cantidad de productos de la instancia " << instance_number << ": ";
			cin >> n;

			cout << "\n   Umbral de peligrosidad de la instancia " << instance_number << ": ";
			int m;
			cin >> m;

			myfile << n;
			myfile << " ";
			myfile << m;
			myfile << " ";
			
			int i = n-1;
			while(i > 0){
				for (int j = 1; j <= i; j++){
					int p = 0;
					p = rand()%50;
					myfile << p;
					myfile << " ";
				}

				i--;
			}
			cant--;
		}
			
	}
	myfile << "0\n";	
	myfile.close();

	cout << "\nSe genero correctamente el archivo fuente inputRandomEJ3.txt\n";

	return 0;
}
