#ifndef __GRASP__H__
#define __GRASP__H__

#include <vector>

using namespace std;

typedef vector< vector <float> > Grafo;
typedef vector<int> Solucion;

class Grasp{
    public:
        Grasp(int repeticiones, int alpha, int beta);
        ~Grasp();
        Solucion resolver(const Grafo& g, int k, int n);
        float getPeso();
        void imprimir();

    private:
        bool criterioDeParada(int peso_local, int repeticiones);
        int alpha;
        int beta;
        int repeticiones;
        Solucion solucion;
        int peso;
};

#endif
