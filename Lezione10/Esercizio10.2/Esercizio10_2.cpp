#include <mpi.h>
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

// Migrazione periodica tra processi MPI: il miglior individuo di ogni popolazione
// viene inviato a un processo vicino secondo un ordine casuale circolare
void Migrazione(Popolazione & pop, int rank, int size, Random & rnd, Citta & city){

    vector <int> order(size);

    if(rank==0){
        // Il processo 0 genera una permutazione casuale dei rank,
        // che definisce l'ordine in cui avviene la migrazione
        vector<bool> estratto(size, false);
        for(int i=0; i<size; i++){
            int j;
            do{
                j = (int)(rnd.Rannyu(0,size));
            } while(estratto[j]);
            order[i]=j;
            estratto[j]=true;
        }
    }

    // Il processo 0 distribuisce l'ordine di migrazione a tutti i processi
    MPI_Bcast(order.data(), size, MPI_INT, 0, MPI_COMM_WORLD);

    // ogni processo individua la propria posizione nell'ordine di migrazione
    int my_pos = -1;
    for(int i = 0; i < size; i++){
        if(order[i] == rank) my_pos = i;
    }

    // Il miglior individuo viene inviato al processo successivo nell'ordine
    int destinazione = order[ (my_pos+1) % size ];
    // e ricevuto dal processo precedente nell'ordine (circolare)
    int provenienza = order[ (my_pos-1+size) % size ];

    int N = pop.GetN();

    vector <int> path_send(N);
    vector <int> path_rec(N);

    // Si carica il percorso del miglior individuo della popolazione locale
    for(int i=0; i<N; i++){
        path_send[i] = pop.GetBest().GetCity(i);
    }

    // Scambio simultaneo: ogni processo invia il proprio miglior percorso al successore
    // e riceve il miglior percorso dal predecessore
    MPI_Sendrecv(path_send.data(), N, MPI_INT, destinazione, 0,
                 path_rec.data(), N, MPI_INT, provenienza,   0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Il percorso ricevuto sostituisce il peggior individuo della popolazione locale
    pop.SwapPeggiore(path_rec, city);
}

int main(int argc, char* argv[]){

    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N_city = 110; // numero di città
    const int M_indiv = 600; // numero di individui
    const int N_gen = 10000; // numeri di generazioni
    const int N_migr = 500; // ogni quanto faccio migrazioni

    ifstream capo("cap_prov_ita.dat");
    Citta City;
    City.Initialize_capoluoghi(capo);

    Random rnd;

    // per far funzionare il generatore di numeri casuali
    int seed[4];
    int p1, p2;
    ifstream Primes("../../GenRandom/Primes");
    if(Primes.is_open()){
        Primes >> p1 >> p2;
    } else cerr << "Non riesco ad aprire il file Primes" << endl;
    Primes.close();

    ifstream input("../../GenRandom/seed.in");
    string property;
    if(input.is_open()){
        while(!input.eof()){
            input >> property;
            if(property == "RANDOMSEED"){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                // Ogni processo riceve un seed diverso sommando il proprio rank
                seed[0] += rank;
                rnd.SetRandom(seed, p1, p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;

    cout << "Hello from process " << rank << " di " << size << endl;

    int interaz_int = 0;
    // Solo il processo 0 interagisce con l'utente
    if(rank == 0){ 
        cout << "\nModalita' di simulazione:" << endl;
        cout << "  [0] Processi INDIPENDENTI (senza migrazione)" << endl;
        cout << "  [1] Processi INTERAGENTI  (con migrazione ogni " << N_migr << " gen)" << endl;
        cout << "Scelta: ";
        cin >> interaz_int;
        cout << "Avvio simulazione..." << endl;
    }
    // Propaghiamo la scelta a tutti i processi
    MPI_Bcast(&interaz_int, 1, MPI_INT, 0, MPI_COMM_WORLD);
    bool interaz = (interaz_int == 1);

    // Apertura file di output dopo aver ricevuto la scelta
    ofstream out_L, out_HL;
    if(interaz){
        out_L.open("OUTPUT/Fitness_capo"      + to_string(rank) + ".dat");
        out_HL.open("OUTPUT/HalfFitness_capo" + to_string(rank) + ".dat");
    } else {
        out_L.open("OUTPUT_SOLO/Fitness_capo"      + to_string(rank) + ".dat");
        out_HL.open("OUTPUT_SOLO/HalfFitness_capo" + to_string(rank) + ".dat");
    }

    Popolazione Pop;
    Pop.Initialize(N_city, M_indiv, City, rnd);

    for(int i=0; i<N_gen; i++){
        if(interaz){
            // Migrazione periodica: ogni N_migr generazioni (non alla gen 0)
            if(i > 0 && i % N_migr == 0){
                Migrazione(Pop, rank, size, rnd, City);
            }
        }
        Pop.Step(City, out_L, out_HL, rnd);
    }

    out_L.close();
    out_HL.close();

    double best_L = Pop.GetBest().GetFiness();
    cout << "Processo " << rank << " best fitness: " << best_L << endl;

    // Raccolta del miglior percorso da tutti i processi:
    vector<int> path_local(N_city);
    for(int i=0; i<N_city; i++) path_local[i] = Pop.GetBest().GetCity(i);

    vector<int> all_paths;
    if(rank == 0) all_paths.resize(size * N_city); // solo il processo 0 alloca il buffer di ricezione

    // raccoglie i percorsi di tutti i processi nel processo 0
    MPI_Gather(path_local.data(), N_city, MPI_INT,
               all_paths.data(), N_city, MPI_INT,
               0, MPI_COMM_WORLD);


    // Raccolta della fitness finale da tutti i processi:
    vector<double> all_fitness;
    if(rank == 0) all_fitness.resize(size); // solo il processo 0 alloca il buffer delle fitness

    // raccoglie la fitness finale di ogni processo nel processo 0
    MPI_Gather(&best_L, 1, MPI_DOUBLE,
               all_fitness.data(), 1, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    
    // processo 0 stampa su file
    if(rank == 0){
        ofstream out_final;
        if(interaz) out_final.open("OUTPUT/best_paths.dat");
        else        out_final.open("OUTPUT_SOLO/best_paths.dat");

        ofstream out_fitness;
        if(interaz) out_fitness.open("OUTPUT/best_fitness_all.dat");
        else        out_fitness.open("OUTPUT_SOLO/best_fitness_all.dat");

        for(int r=0; r<size; r++){
            out_final << r << "\t";
            for(int i=0; i<N_city; i++){
                out_final << " " << all_paths[r*N_city + i]; // accede al percorso del processo r nell'array piatto
            }
            out_final << endl;

            out_fitness << r << "\t" << all_fitness[r] << endl; // scrive rank e fitness del processo r
        }

        out_final.close();
        out_fitness.close();
    }

    MPI_Finalize();

    return 0;
}