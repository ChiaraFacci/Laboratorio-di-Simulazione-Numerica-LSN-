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


// ----- INIZIO ESERCIZIO -------------------------------------------

    int M=10000;
    int Nv[4]={1,2,10,100};

        
for (int j = 0; j < 4; j++) {          // itera sui 4 valori di N
    int N = Nv[j];
    string nomefile = "dati" + to_string(N) + ".txt";
    ofstream stampa(nomefile);

    for (int k = 0; k < M; k++) {       // M=10000 realizzazioni
        double accuU = 0, accuE = 0, accuL = 0;

        for (int i = 0; i < N; i++) {   // somma N campioni
            accuU += rnd.Rannyu(1., 7.);
            accuE += rnd.Exponential(1.);
            accuL += rnd.Lorentzian(1., 0.);
        }

        stampa << accuU/N << '\t' << accuE/N << '\t' << accuL/N << '\n';
    }
}

    return 0;
}