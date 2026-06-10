#ifndef __Individuo__
#define __Individuo__

#include <iostream>
#include <fstream>
#include "Citta.h"
#include "../../GenRandom/random.h"
#include <vector>

using namespace std;

class Individuo {
    private:
    int _dim; // numero di città
    vector<int> _path; // vector che contiene il path ordinato delle _dim città
    double _L; // fitness
    double _p_mut1 = 0.1; // prob di mutaz 1
    double _p_mut2 = 0.1; // prob di mutaz 2
    double _p_mut3 = 0.1; // prob di mutaz 3
    double _p_mut4 = 0.1; // prob di mutaz 4

    public:
    Individuo() : _dim(0), _L(0.0) {} // default constructor

    // inizializza individuo estraendo casualmente il path
    void Initialize(int N, Citta & city, Random & rnd);
    
    // imposta un _path diverso
    void SetPath(vector<int>path);
    
    // restituisce il numero di città _dim
    int GetDim();
    // imposta il numero di città _dim
    void SetDim(int N);
    // restiìuisce la città i-esima
    int GetCity(int i);
    // imposta la città i-esima
    void SetCity(int i, int u);

    // calcola la fitness
    void Fitness(Citta & city);
    // resituisce la fitness
    double GetFiness();
    // imposta la fitness
    void SetFitness(double L);

    // mappa l'indice i come se path fosse un anello circolare
    int pbc(int i);
    void Mutazione1(Random & rnd);      // scambia 2 città casuali
    void Mutazione2(Random & rnd);      // shifta m città di n posizioni
    void Mutazione3(Random & rnd);      // scambia 2 blocchi contigui di m città  
    void Mutazione4(Random & rnd);      // inverte l'ordine di m città

    // attiva le mutazioni ognuna con la sua probabilità
    void AttivaMutazioni(Random & rnd);

    // stampa il path
    void Stampa(ofstream & fileout);
    // stampa la fitness
    void StampaFitness(ofstream & fileout);

    // controlla che l'individuo sia sano e arresta la simulaz altrimenti
    bool Check();

};

#endif
