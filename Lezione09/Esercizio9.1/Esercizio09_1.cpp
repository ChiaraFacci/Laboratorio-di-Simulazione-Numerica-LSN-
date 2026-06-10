#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include "../../GenRandom/random.h"
#include "Citta.h"
#include "Individuo.h"
#include "Popolazione.h"

using namespace std;

int main(){

    const int N_city = 34; // numero di città
    const int M_indiv = 100; // numero di individui
    const int N_gen = 1000; // numero di generazione

    ofstream punti_circ("OUTPUT/punti_circ.dat");
    ofstream punti_random("OUTPUT/punti_random.dat");

    ofstream out_path1("OUTPUT/Evoluzione_best_circ.dat");
    ofstream out_L1("OUTPUT/Fitness_circ.dat");
    ofstream out_HL1("OUTPUT/HalfFitness_circ.dat");

    ofstream out_path2("OUTPUT/Evoluzione_best_random.dat");
    ofstream out_L2("OUTPUT/Fitness_random.dat");
    ofstream out_HL2("OUTPUT/HalfFitness_random.dat");
    

    // stampo su file le posizione delle città dei due percorsi
    Citta City1;
    Citta City2;

    City1.Initialize_circ(N_city);
    City2.Initialize_random(N_city);

    for(int i=0; i<N_city; i++){
        punti_circ << setw(16) << City1.GetPosX(i) 
        << setw(16) << City1.GetPosY(i)<< endl;

        punti_random << setw(16) << City2.GetPosX(i) 
        << setw(16) << City2.GetPosY(i) << endl; 
    }


    // creo, inizializzo ed evolvo le due popolazioni
    Popolazione Pop1;
    Popolazione Pop2;
    
    Pop1.Initialize(N_city, M_indiv, City1);
    Pop2.Initialize(N_city, M_indiv, City2);

    for(int i=0; i<N_gen; i++){
        Pop1.Step(City1, out_path1, out_L1, out_HL1);
        Pop2.Step(City2, out_path2, out_L2, out_HL2);
    }


    return 0;
}