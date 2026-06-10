#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include "Citta.h"

using namespace std;

void Citta::Initialize_circ(int N){
  
    // inizializzo il gen di numeri casuali
    int seed[4];
    int p1, p2;
    ifstream Primes("../../GenRandom/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
        Primes >> p1 >> p2 ;
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
            _rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;


    _dim=N;

    for(int i=0; i<_dim; i++){
        double theta = _rnd.Rannyu(0, 2*M_PI);

        _pos_x.push_back(cos(theta));
        _pos_y.push_back(sin(theta));

    }

}

void Citta::Initialize_random(int N){
  
    // inizializzo il gen di numeri casuali
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
            _rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;

    _dim=N;

    for(int i=0; i<_dim; i++){
        double x = _rnd.Rannyu(0,1);
        double y = _rnd.Rannyu(0,1);

        _pos_x.push_back(x);
        _pos_y.push_back(y);
    }

}

void Citta:: Initialize_capoluoghi(ifstream & filein){
    double x,y;
    while(!filein.eof()){
        filein >> x >> y;
        
        _pos_x.push_back(x);
        _pos_y.push_back(y);
    }

    _dim = _pos_x.size();
}

double Citta:: GetPosX(int i){
    return _pos_x[i];
}

double Citta:: GetPosY(int i){
    return _pos_y[i];
}

double Citta::Distanza(int i, int j) {
    double dx = _pos_x[i-1] - _pos_x[j-1];
    double dy = _pos_y[i-1] - _pos_y[j-1];

    return sqrt(dx*dx + dy*dy);
}
