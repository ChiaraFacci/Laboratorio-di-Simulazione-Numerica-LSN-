#ifndef __Popolazione__
#define __Popolazione__

#include <iostream>
#include <fstream>
#include "Citta.h"
#include "../../GenRandom/random.h"
#include <vector>
#include "Individuo.h"

using namespace std;

class Popolazione {
    private:
    vector<Individuo> _popolaz; // vector di individui
    Individuo _best_individ; // miglior individuo
    double _p_crossover=0.7; // prob di crossover
    double _p_sel= 2; // fattore di selezione
    int _N_citta; // numero di città
    int _M_indiv; // numero di individui
    Random _rnd; // generatore di numeri casuali


    public:
    // inizializza una popolazione di M individui su N cittè disposte secondo city
    void Initialize(int N, int M, Citta & city);
    // seleziona con la legge int(M*r^p)
    int Seleziona();
    
    // ordina in base a fitness: i migliori stanno davanti
    void SortFitness();
    // calcola la fitness media sulla metà migliore della popolaz
    double GetHalfFitness();

    // fa crossover tra madre e padre
    pair<Individuo, Individuo> Crossover(Individuo& madre, Individuo& padre);
    // crea una nuova generazione con crossover e mutazioni
    void NuovaGen(Citta & city);

    // crea nuova generazione e stampa su file
    void Step(Citta & city, ofstream & outbest, ofstream & outFit, ofstream & outHalfFit);



};

#endif