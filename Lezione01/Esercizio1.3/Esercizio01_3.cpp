#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "../../GenRandom/random.h"

using namespace std;

int main(){

    Random rnd;

    // per far funzione il generatore di numeri casuali
    int seed[4];
    int p1, p2;
    ifstream Primes("../../GenRandom/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
        Primes >> p1 >> p2 ;
    } else cerr << "Non riesco ad aprire il file Primes" << endl;
    Primes.close();

    ifstream input("../../GenRandom/seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;


    // ------- INIZIO ESERCIZIO -------------------------

    int N=100; // numero di blocchi
    int M=100000; // numero lanci per blocco

    double L=0.9; // lunghezza ago
    double d=1.; // distanza tra le righe

    ofstream stampa("dati.txt");

    double somma=0;
    double quad=0;
    for(int j=0; j<N; j++){

        int accu=0;
        for(int i=0; i<M; i++){
            double x0=rnd.Rannyu(0,d);
            double theta=rnd.Rannyu(0,M_PI);
            double dist = min(x0, d-x0);

            if(dist<=(L/2.0)*sin(theta)){
                accu++;
            }

        }
        double P=2*L*M/(d*accu);

        somma+=P;
        quad+=pow(P,2);

        double pi= somma/(j+1);
        double pi2=quad/(j+1);
        double devstd= sqrt((pi2-pow(pi,2))/(j+1));

        stampa << j+1 <<  "\t" << pi << "\t" << devstd << endl;

    }

    stampa.close();

    return 0;
}