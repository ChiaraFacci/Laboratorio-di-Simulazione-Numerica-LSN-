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
    double _p_crossover=0.8; // prob di crossover
    double _p_sel= 2; // fattore di selezione
    int _N_citta; // numero di città
    int _M_indiv; // numero di individui


    public:
    // inizializza una popolazione di M individui su N cittè disposte secondo city
    void Initialize(int N, int M, Citta & city, Random & _rnd);
    // seleziona con la legge int(M*r^p)
    int Seleziona(Random & _rnd);
    
    // ordina in base a fitness: i migliori stanno davanti
    void SortFitness();
    // calcola la fitness media sulla metà migliore della popolaz
    double GetHalfFitness();

    // fa crossover tra madre e padre
    pair<Individuo, Individuo> Crossover(Individuo& madre, Individuo& padre, Random & _rnd);
    // crea una nuova generazione con crossover e mutazioni
    void NuovaGen(Citta & city, Random & _rnd);

    // crea nuova generazione e stampa su file
    void Step(Citta & city, ofstream & outFit, ofstream & outHalfFit, Random & _rnd);

    // restituisce il numero di città N
    int GetN();
    // restituisce il numero di individui M
    int GetM();

    // restituisce il miglior indivisuo
    Individuo GetBest();

    // scambia l'individuo peggiore con quello ricevuto
    void SwapPeggiore(vector<int> path, Citta & city);

};

#endif
