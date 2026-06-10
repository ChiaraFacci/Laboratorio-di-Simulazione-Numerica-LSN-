#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "../../GenRandom/random.h"

using namespace std;

double error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

int main(){

    Random rnd;

    // per far funzione il generatore di numeri casuali
    int seed[4];
    int p1, p2;
    ifstream Primes("../../GenRandom/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2;
        Primes >> p1 >> p2;
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

//---- PUNTO 1: metodo diretto --------------------------------------------------

    double S0 = 100.;
    double T=1.;
    double K=100.;
    double r=0.1;
    double sigma=0.25;
    double N=100;
    double M=50000;
    double L=M/N;


    ofstream stampaC1("datiC1.txt");
    ofstream stampaP1("datiP1.txt");


    double sommaC=0; double sommaP=0;
    double quadC=0; double quadP=0;

    for(int i=0; i<N; i++){
        
        double accuC=0;
        double accuP=0;

        for(int j=0; j<L; j++){
            double Z=rnd.Gauss(0,1.);
            double S=S0*exp((r-0.5*pow(sigma,2))*(T)+sigma*Z*sqrt(T));
            double C=exp(-r*T)*max(0., S-K);
            double P=exp(-r*T)*max(0.,K-S );
            accuC+=C;
            accuP+=P;
        }

        sommaC+= accuC/L; sommaP+=accuP/L;
        quadC += pow(accuC/L,2); quadP += pow(accuP/L,2);

        stampaC1 << i+1 << "\t" << sommaC/(i+1) << "\t" << error(sommaC, quadC, i+1) << endl;
        stampaP1 << i+1 << "\t" << sommaP/(i+1) << "\t" << error(sommaP, quadP, i+1) << endl;
    }

// ---- PUNTO 2: metodo indiretto ----------------------------------------------

    double dt=T/100;

    ofstream stampaC2("datiC2.txt");
    ofstream stampaP2("datiP2.txt");

    double sommaC2=0; double sommaP2=0;
    double quadC2=0; double quadP2=0;

    for(int i=0; i<N; i++){
        
        double accuC=0;
        double accuP=0;

        for(int j=0; j<L; j++){
            
            double S=S0;
            for(int k=0; k<100; k++){
                double Z=rnd.Gauss(0,1.);
                // ogni volta moltiplico per quello nuovo
                S *= exp((r-0.5*pow(sigma,2))*(dt)+sigma*Z*sqrt(dt));
            }

            double C=exp(-r*T)*max(0., S-K);
            double P=exp(-r*T)*max(0., K-S);
            accuC+=C;
            accuP+=P;
        }

        sommaC2+= accuC/L; sommaP2+=accuP/L;
        quadC2 += pow(accuC/L,2); quadP2 += pow(accuP/L,2);

        stampaC2 << i+1 << "\t" << sommaC2/(i+1) << "\t" << error(sommaC2, quadC2, i+1) << endl;
        stampaP2 << i+1 << "\t" << sommaP2/(i+1) << "\t" << error(sommaP2, quadP2, i+1) << endl;
    }    

    return 0;

}