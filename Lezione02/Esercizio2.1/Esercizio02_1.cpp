#include <iostream>
#include <fstream>
#include <cmath>
#include "../../GenRandom/random.h"

using namespace std;


double error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

int main(){


// ----- inizializzo generatore di numeri casuali ----------------

    Random gen;

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
            gen.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;




    int N=100; //blocchi
    int M=10000; // lanci
    int L= M/N; // lanci per blocco
     
    ofstream stampa("uniforme.dat");
    ofstream stampa2("sampling.dat");

    double I_unif=0; double J=0;
    double I_sampl=0; double J2=0;

    //questo for fa la media cumulativa per ogni blocco
    for(int j=0; j<N; j++){

        double f1=0; double f2=0;
        
        //questo for calcola l'integrale
        for(int i=0; i<L; i++){
            double x=gen.Rannyu();
            
            //metodo uniforme
            f1 += M_PI*cos(M_PI*x/2)/2;
            
            // importance sampling
            double y = 1 - sqrt(1 - x);
            f2 += (M_PI/2 * cos(M_PI*y/2)) / (2*(1-y));
        }

        double b1= f1/L;   
        double b2=f2/L;

        I_unif+=b1;     I_sampl+=b2;   
        J+=b1*b1;       J2+=b2*b2;
  

        stampa << j+1 << "\t" << I_unif/(j+1) << "\t" << error(I_unif,  J,  j+1) << endl;
        stampa2 << j+1 << "\t" << I_sampl/(j+1) << "\t" << error(I_sampl,  J2,  j+1) << endl;
    }

    return 0;
}