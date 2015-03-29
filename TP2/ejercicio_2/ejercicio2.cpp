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
bool everyKnightInSamePlace(Knights& knights);

int main(){
	int n;
	int k;
	int x;
	int y;
	Coord c;
	Knights knights;

	cout << "Insert size of matrix: ";
	cin >> n;
	cout << "Insert amount of knights: ";
	cin >> k;
	knights.reserve(k);

	for (int i = 0; i < k; ++i){
		printf("Insert knight %d row: ", i+1);
		cin >> x;
		printf("Insert knight %d column: ", i+1);
		cin >> y;
		c = make_pair(x-1, y-1);
		knights.push_back(c);
	}
	
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

	//analysis if the size of the matrix is lower than 3x3, in that case it could not have a solution
	if(n > 3){
		printf("%d %d %d", min_coord.first+1, min_coord.second+1, min_distance);
		return 0;
	} else {

		if(n == 3){
			bool in_center = false;
			for (int i = 0; i < knights.size(); ++i){
				in_center = (knights[i].first == 1 && knights[i].second == 1) || in_center;
			}
			//if there are any knights in center => every knight should be in the center
			if(in_center){
				if(everyKnightInSamePlace(knights)){
					printf("%d %d %d", knights[0].first+1, knights[0].second+1, 0);
					return 0;
				} else {
					cout << "no" <<  endl;
					return 0;
				}
			//there are no knights in the center, so the algorithm has resolved it correctly
			} else {
				printf("%d %d %d", min_coord.first+1, min_coord.second+1, min_distance);
				return 0;
			}
		}

		if(n == 1){
			printf("%d %d %d", knights[0].first+1, knights[0].second+1, 0);
			return 0;
		}

		//if size of the matrix is 2x2, then the only solution is if all the knights are in the same position
		if(n == 2 && everyKnightInSamePlace(knights) ){
			printf("%d %d %d", knights[0].first+1, knights[0].second+1, 0);
			return 0;
		} else {
			cout << "no" << endl;
			return 0;
		}		
	}

}

/*void show(const Matrix& m){
	printf("\n");
	for (int i = 0; i < m.size(); ++i){
		for (int j = 0; j < m[i].size(); ++j){
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
}*/

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

bool everyKnightInSamePlace(Knights& knights){
	bool same_place = true;
	Coord c = make_pair(knights[0].first, knights[0].second);

	for (int k = 1; k < knights.size(); ++k){
		same_place = c.first == knights[k].first && c.second == knights[k].second && same_place;
	}

	return same_place;
}