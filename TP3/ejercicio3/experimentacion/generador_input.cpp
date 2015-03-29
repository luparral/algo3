#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

bool esta(int q, int p, vector<vector<int> >& aristas);

int main(){
	ofstream myfile ("random.txt");
	
	// cout << "\nIngresa cuantas instancias deseas generar: ";
	int cant = 1;
	// cin >> cant;
	
	if(myfile.is_open()){
		int instance_number = 0;
		srand ( time(NULL) );
		while(cant > 0){
			instance_number++;
			int n;
			//cout << "\n   Cantidad de nodos de la instancia " << instance_number << ": ";
			cin >> n;

			//cout << "\n   Cantidad de aristas de la instancia " << instance_number << ": ";
			int m;
			cin >> m;

			//cout << "\n   Dame el k de la instancia " << instance_number << ": ";
			int k;
			cin >> k;

			myfile << n;
			myfile << " ";
			myfile << m;
			myfile << " ";
			myfile << k;
			myfile << "\n";

			vector<vector<int> > aristas;
			int p = 0;

			for (int i = 0; i < m; ++i){
				
				p = rand()%(n);
				//while(p == 0 || p == n+1)
					//p = rand()%n;

				myfile << p;
				myfile << " ";

				int q = p;
				while(p== 0 || q == p || esta(q,p,aristas))
					p = rand()%n;

				vector<int> nuevaArista;
				nuevaArista.push_back(q);
				nuevaArista.push_back(p);
				aristas.push_back(nuevaArista);

				myfile << p;
				myfile << " ";

				p = rand()%100;
				myfile << p;
				myfile << "\n";
			}
			cant--;
		}
			
	}	
	myfile.close();

	//cout << "\nSe genero correctamente el archivo fuente random.txt\n";

	return 0;
}

bool esta(int q, int p, vector<vector<int> >& aristas){
	vector<int> nuevaArista;
	nuevaArista.push_back(q);
	nuevaArista.push_back(p);

	vector<int> nuevaAristaInvertida;
	nuevaAristaInvertida.push_back(p);
	nuevaAristaInvertida.push_back(q);

	for (int i = 0; i < aristas.size(); ++i)
		if(nuevaArista == aristas[i] || nuevaAristaInvertida == aristas[i])
			return true;

	return false;
}
