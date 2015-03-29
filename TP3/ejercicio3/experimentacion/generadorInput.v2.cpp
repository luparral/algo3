#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

struct Arista {
	int n1;
	int n2;
	
	Arista(int x, int y):n1(x), n2(y){}

	bool	operator==(const Arista& a2) {
		return (n1 == a2.n1 && n2 == a2.n2)	|| (n1 == a2.n2 && n2 == a2.n1);	
	}
};

bool esta(int q, int p, vector<Arista>& aristas);
int grado(int p, vector<Arista>& aristas);

int main(){
	ofstream myfile ("random.txt");

	//cout << "\nIngresa cuantas instancias deseas generar: ";
	int cant;
	cin >> cant;
	
	if(myfile.is_open()){
		int instance_number = 0;
		srand ( time(NULL) );
		while(cant > 0){
			instance_number++;
			int n, m, k;
			//cout << "\n   Cantidad de nodos de la instancia " << instance_number << ": ";
			cin >> n;

			//cout << "\n   Cantidad de aristas de la instancia " << instance_number << ": ";
			cin >> m;

			//cout << "\n   Dame el k de la instancia " << instance_number << ": ";
			cin >> k;

			myfile << n << " " << m << " " << k << "\n";

			vector<Arista> aristas;
			int p = 0;

			for (int i = 0; i < m; ++i){

				p = (rand()% n)+1;
				while(grado(p,aristas) == n-1){
					p = (rand()%n)+1;
				}

				myfile << p;
				myfile << " ";

				int q = p;
				while(q == p || esta(q,p,aristas)){
					p = (rand()%n)+1;
				}

				Arista nuevaArista(q, p);
				aristas.push_back(nuevaArista);

				myfile << p;
				myfile << " ";

				p = rand()%100;
				myfile << p;
				myfile << "\n";
			}
			cant--;
		}
		//myfile << "0" << endl;
			
	}	
	myfile.close();

	//cout << "\nSe genero correctamente el archivo fuente random.txt\n";

	return 0;
}

bool esta(int q, int p, vector<Arista>& aristas){
	Arista nuevaArista(q,p);

	for (int i = 0; i < aristas.size(); ++i)
		if(nuevaArista == aristas[i])
			return true;

	return false;
}

int grado(int p, vector<Arista>& aristas){
	int res = 0;
	for (int i = 0; i < aristas.size(); ++i){
		if(aristas[i].n1 == p)
			res++;
		if(aristas[i].n2 == p)
			res++;
	}
	return res;
}
