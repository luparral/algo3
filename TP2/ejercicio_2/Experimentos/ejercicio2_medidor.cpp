#include <iostream>
#include <vector>
#include <queue>
#include <stdio.h> //printf
#include <time.h>
#include <ctime>

using namespace std;

typedef vector< vector<int> > Matrix;
typedef pair<int, int> Coord;
typedef vector<Coord> Neighbors;
typedef vector<Coord> Knights;

void distanceMatrix(Matrix& m, Coord c);
void show(const Matrix& m);
Neighbors get_neighbors(const Matrix& m, const Coord c);
bool valid(const Matrix& m, const Coord c);
void sum(Matrix& a, const Matrix& b);

int main(){
	int n;
	int k;
	int instancias;
	cin >> instancias;
	//cout << "Insert size of matrix: ";
	cin >> n;
	//cout << "Insert amount of knights: ";
	cin >> k;
	printf("%d instancias de tamaño n = %d y k = %d \n", instancias, n, k);

	double acum_minimos = 0;;

	while(n != 0){

		
		int x;
		int y;
		Coord c;
		Knights knights;
		clock_t t_ini, t_fin;
		double secs;		
				
		knights.reserve(k);

		for (int i = 0; i < k; ++i){
			//printf("Insert knight %d coordenate x axis: ", i);
			cin >> x;
			//printf("Insert knight %d coordenate y axis: ", i);
			cin >> y;
			c = make_pair(x-1, y-1);
			knights.push_back(c);
		}
		
		//repito j veces cada instancia para calcularle el minimo
		int repeticiones = 20;
		int it = repeticiones;
		double menorTiempo = 99999999;
		double acumulador = 0;
		while(it > 0){


			t_ini = clock();

			vector<int> v(n, 0);
			Matrix cache(n, v);

			for (int i = 0; i < knights.size(); ++i){
				vector<int> v(n, -1);
				Matrix m(n, v);

				distanceMatrix(m, knights[i]);
				sum(cache, m);

				//show(m);
			}

			//show(cache);

			//search for minimum distance
			int min_distance = cache[0][0];
			Coord min_coord(0, 0);

			for (int i = 0; i < cache.size(); ++i){
				for (int j = 0; j < cache[i].size(); ++j){
					if(cache[i][j] < min_distance){
						min_distance = cache[i][j];
						min_coord = make_pair(j, i);
					}
				}
			}

			t_fin = clock();
			secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
			acumulador += secs;
			if(menorTiempo > secs) menorTiempo = secs;
			it--;
		}

		printf("Tiempo minimo: %f, Tiempo promedio: %f\n", menorTiempo, acumulador/repeticiones);
		//printf("Coordenate: %d %d  min distance: %d", min_coord.second+1, min_coord.first+1, min_distance);

		acum_minimos += menorTiempo;
		//cout << "Insert size of matrix: ";
		cin >> n;
		cin >> k;
	}

	printf("Promedio de tiempos minimos: %f", acum_minimos/instancias);

	return 0;
}

void show(const Matrix& m){
	printf("\n");
	for (int i = 0; i < m.size(); ++i){
		for (int j = 0; j < m[i].size(); ++j){
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
}

void distanceMatrix(Matrix& m, Coord c){
	m[c.second][c.first] = 0;

	queue<Coord> q;
	q.push(c);

	while(!q.empty()){
		Coord actual = q.front();

		Neighbors neighbors = get_neighbors(m, actual);

		for (int i = 0; i < neighbors.size(); ++i){
			Coord aux = neighbors[i];
			//nunca se recorrio aun esta posicion
			if(m[aux.second][aux.first] == -1){
				m[aux.second][aux.first] = m[actual.second][actual.first] + 1;
				q.push(aux);
			} else 
			if(m[aux.second][aux.first] > m[actual.second][actual.first] + 1){
				m[aux.second][aux.first] = m[actual.second][actual.first] + 1;
			}
		}

		q.pop();
	}

	
}

Neighbors get_neighbors(const Matrix& m, const Coord c){
	Neighbors result;
	result.reserve(8);
	
	//checkeo todos los adyacentes
	Coord c2 (c.first - 1, c.second + 2);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first + 1, c.second + 2);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first - 1, c.second - 2);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first + 1, c.second - 2);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first + 2, c.second + 1);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first + 2, c.second - 1);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first - 2, c.second + 1);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	c2 = make_pair(c.first - 2, c.second - 1);
	if(valid(m, c2)){
		result.push_back(c2);
	}
	
	return result;
}

bool valid(const Matrix& m, const Coord c){
	int limit = m.size();
	return (c.first < limit) && (c.first >= 0) && (c.second < limit) && (c.second >= 0);
}

void sum(Matrix& a, const Matrix& b){
	for (int i = 0; i < a.size(); ++i){
		for (int j = 0; j < a[i].size(); ++j){
			a[i][j] += b[i][j];
		}
	}
}