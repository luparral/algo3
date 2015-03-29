#include <utility>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm> 
#include <limits>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <time.h>

using namespace std;

int main(){

	string a;
	string b;
	int n;

	cin >> a;
	cin >> b;
	cin >> n;

	if(a == "" || b == "" || n < 2){
		cout << "Hay un problema en A, B o N" << endl;
		return 0;
	}

	string o;
	string d;
	int i;
	int f;

	int iter = 1;
	while(iter <= n){
		cin >> o;
		cin >> d;
		cin >> i;
		cin >> f;

		if(o == ""){
			cout << "ORIGEN VACIO en vuelo " << i << endl;
			break;
		}

		if(d == ""){
			cout << "DESTINO VACIO en vuelo " << i << endl;
			break;
		}

		if(o == d){
			cout << "ORIGEN = DESTINO en vuelo " << i << endl;
			break;
		}

		if(i >= f){
			cout << "INICIO MAYOR O IGUAL A FIN en vuelo " << iter+1 << endl;
			break;
		}

		iter++;
		if(n < iter){
			cout << "TODO OK LA INSTANCIA" << endl;
		}
	}

	return 0;
}