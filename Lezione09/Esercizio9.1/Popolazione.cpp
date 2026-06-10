#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "Citta.h"
#include "Individuo.h"
#include "Popolazione.h"

using namespace std;

// inizializza una popolazione di M individui su N cittè disposte secondo city
void Popolazione::Initialize(int N, int M, Citta & city){
    
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

    _N_citta = N;
    _M_indiv = M;

    _popolaz.resize(_M_indiv);

    for(int i=0; i<_M_indiv; i++){
        _popolaz[i].Initialize(_N_citta, city, _rnd);
    }

    SortFitness();

    for(int i=0; i<_M_indiv; i++) _popolaz[i].Check();

    _best_individ = _popolaz[0];

}

// ordina in base a fitness: i migliori stanno davanti
void Popolazione:: SortFitness(){
    
    for(int i=0; i<_M_indiv; i++){
        int i_min =i;

        for(int j=i+1; j<_M_indiv; j++){
            if(_popolaz[j].GetFiness() < _popolaz[i_min].GetFiness()){
                i_min = j;
            }

        
        }
        swap(_popolaz[i], _popolaz[i_min]);
    }
}


// seleziona con la legge int(M*r^p)
int Popolazione:: Seleziona(){
    double r = _rnd.Rannyu(0,1);
    return (int)(_M_indiv * pow(r,_p_sel));
}


// fa crossover
pair<Individuo, Individuo> Popolazione:: Crossover(Individuo& madre, Individuo& padre){
    int N = madre.GetDim();
    int cut = (int)(_rnd.Rannyu(2, N)); // agli estremi ho 1 elem da un genitore e tutto il resto dall'altro


// figlio1 è uguale a madre fino al cut, dopo riempe con gli elementi restanti nell'ordine in cui appaiono in padre
    
    Individuo f1;
    f1.SetDim(N);
    f1.SetFitness(0.0);
    vector<int> path1(N);

    // copia gli elementi di madre fino al cut
    for(int i=0; i<cut; i++){
        path1[i]=madre.GetCity(i);
    }

    // inserisce i rimamenti con l'ordine di padre
    vector <bool> used1(N+1,false);
    
    for(int i=0; i<cut; i++) used1[path1[i]]=true;

    int index1 = cut;
    for(int i=0; i<N; i++){
        int u = padre.GetCity(i);
        if(!used1[u]){
            path1[index1]=u;
            used1[u]=true;
            index1++;
        }
    }

    f1.SetPath(path1);


// figlio2 è uguale a padre fino al cut, dopo riempe con gli elementi restanti nell'ordine in cui appaiono in madre

    Individuo f2;
    f2.SetDim(N);
    f2.SetFitness(0.0);
    vector<int> path2(N);

    // copia gli elementi di madre fino al cut
    for(int i=0; i<cut; i++){
        path2[i]=padre.GetCity(i);
    }

    // inserisce i rimamenti con l'ordine di madre
    vector <bool> used2(N+1,false);

    for(int i=0; i<cut; i++) used2[path2[i]]=true;

    int index2 = cut;
    for(int i=0; i<N; i++){
        int u = madre.GetCity(i);
        if(!used2[u]){
            path2[index2]=u;
            used2[u]=true;
            index2++;
        }
    }

    f2.SetPath(path2);
    
    f1.Check();
    f2.Check();
    
    return {f1, f2};
}

// crea una nuova generazione con crossover e mutazioni
void Popolazione::NuovaGen(Citta& city){
    vector<Individuo> newgen;
    newgen.reserve(_M_indiv);

    while((int)newgen.size() < _M_indiv){

        if(_rnd.Rannyu(0,1) < _p_crossover){
            int index_mum = Seleziona();
            int index_dad = Seleziona();
            while(index_mum == index_dad) index_dad = Seleziona();

            auto [f1,f2] = Crossover(_popolaz[index_mum], _popolaz[index_dad]);

            f1.AttivaMutazioni(_rnd);
            f2.AttivaMutazioni(_rnd);

            f1.Fitness(city);
            f2.Fitness(city);

            newgen.push_back(f1);
            if((int)newgen.size() < _M_indiv) newgen.push_back(f2);

        } else {
            int index = Seleziona();
            Individuo f = _popolaz[index];
            
            f.AttivaMutazioni(_rnd);
            f.Fitness(city);
            
            newgen.push_back(f);
        }
    }

    _popolaz = newgen;

    for(int i=0; i<_M_indiv; i++){
        newgen[i].Fitness(city);
    }
   
    SortFitness();
    _best_individ = _popolaz[0];

}


// calcola la fitness media sulla metà migliore
double Popolazione:: GetHalfFitness(){

    double media=0.0;
    int half = _M_indiv/2;

    for(int i=0; i<half; i++){
        media += _popolaz[i].GetFiness();
    }

    media /= half;

    return media;
}

// crea nuova generazione e stampa su file
void Popolazione::Step(Citta & city, ofstream & outbest, ofstream & outFit, ofstream & outHalfFit){
    
    NuovaGen(city);
    
    _best_individ.Stampa(outbest);
    _best_individ.StampaFitness(outFit);

    outHalfFit << GetHalfFitness() << endl;

}