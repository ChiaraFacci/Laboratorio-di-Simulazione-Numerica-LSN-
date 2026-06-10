#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "Citta.h"
#include "Individuo.h"

using namespace std;

// inizializza individuo estraendo casualmente il path
void Individuo::Initialize(int N, Citta & city, Random & rnd){
 
    _dim=N;

    _path.resize(_dim);
    _path[0] = 1; // la prima città è sempre fissata


    vector<bool> estratto(N+1, false);  // indici 0..N
    estratto[1] = true;                 // blocca la città 1 che è già in pos 0

    for(int i=1; i<_dim; i++){
        
        int u;
        do {
            u = (int)(rnd.Rannyu(2, _dim+1));  // estrae in {2, N}
        } while(estratto[u]);
        
        _path[i] = u;
        estratto[u] = true;
    }

    Fitness(city);
}


void Individuo::SetPath(vector<int>path){
    _path = path;
};

int Individuo::GetDim(){
    return _dim;
}

void Individuo::SetDim(int N){
    _dim = N;
}

int Individuo:: GetCity(int i){
    return _path[i];
}

void Individuo:: SetCity(int i, int u){
    _path[i] = u;
}


// calcola la fitness
void Individuo::Fitness(Citta & city){
    _L=0;

    for(int i=0; i<_dim; i++){
        int a = _path[i];
        int b = _path[(i+1) % _dim]; // % _dim fa modulo _dim -> quindi quando i è l'ultima città qui c'è la prima

        _L += city.Distanza(a,b);
    }
}

double Individuo:: GetFiness(){
    return _L;
}

void Individuo::SetFitness(double L){
    _L=L;
}


// mappa l'indice i come se path fosse un anello circolare
int Individuo:: pbc(int i){
    if(i >= _dim) return i - (_dim - 1); // sforo a dx
    if(i < 1)     return i + (_dim - 1); // sforo a sx
    return i; // intervallo corretto
}

// scambia 2 città casuali
void Individuo:: Mutazione1(Random & rnd){
    
    int a = (int)(rnd.Rannyu(1,_dim)); // indice prima città
    int b; // indice seconda città

    do { b = (int)(rnd.Rannyu(1, _dim)); } while(b == a);
    swap(_path[a], _path[b]);
}

// shifta m città di n posizioni
void Individuo::Mutazione2(Random& rnd){
    int m     = (int)(rnd.Rannyu(1, _dim - 2)); // m in {1, dim-3} -> almeno 1 posto prima e dopo al blocco
    int start = (int)(rnd.Rannyu(1, _dim - m)); // start in {1, dim-m-1}
    int max = _dim - (start + m);
    int n = (int)(rnd.Rannyu(1, max+1)); // n in {1, max}

    vector<int> shifted;
    shifted.reserve(_dim);

    // copia uguale gli elementi prima di start
    for(int i=0; i<start; i++) shifted.push_back(_path[i]);

    // inserisce gli n elementi saltati
    for(int i=start+m; i<start+m+n; i++) shifted.push_back(_path[i]);

    // inserisce il blocco spostato
    for(int i=start; i<start+m; i++) shifted.push_back(_path[i]);

    // copia uguale gli ultimi elementi invariati
    for(int i=start+m+n; i<_dim; i++) shifted.push_back(_path[i]);

    _path = shifted;
}

// scambia 2 blocchi contigui di m città 
void Individuo:: Mutazione3(Random & rnd){
  
int m = 1 + (int)(rnd.Rannyu(0, (_dim - 1) / 2 - 1));  // m in {1, (dim-1)/2 - 1}
    int pos1, pos2;
    do {
        pos1 = 1 + (int)(rnd.Rannyu(0, _dim - 1)); // inizio del primo blocco
        pos2 = 1 + (int)(rnd.Rannyu(0, _dim - 1)); // inizio del secondo blocco
    
    } while(pos2 >= (pos1 - m) and pos2 <= (pos1 + m));  // evita sovrapposizione

    for(int i=0; i<m; i++)
        swap(_path[pbc(pos1+i)], _path[pbc(pos2+i)]);
   
}

// inverte l'ordine di m città
void Individuo::Mutazione4(Random& rnd){
    int m = (int)(rnd.Rannyu(2, _dim)); // m in {2, dim-1}
    int pos = 1 + (int)(rnd.Rannyu(0, _dim - m)); // pos in {1, size-m-1}

    for(int i=0; i < m/2; i++){
        swap(_path[pos+i], _path[pos+m-1-i]);
    }
}

// attiva le mutazioni ognuna con la sua probabilità
void Individuo:: AttivaMutazioni(Random & rnd){
    if(rnd.Rannyu(0,1)<_p_mut1) {Mutazione1(rnd); Check();}
    if(rnd.Rannyu(0,1)<_p_mut2) {Mutazione2(rnd); Check();}
    if(rnd.Rannyu(0,1)<_p_mut3) {Mutazione3(rnd); Check();}
    if(rnd.Rannyu(0,1)<_p_mut4) {Mutazione4(rnd); Check();}
}


// stampa il path
void Individuo:: Stampa(ofstream & fileout){
    for(int i=0; i<_dim; i++){
        fileout << _path[i] << " ";
    }

    fileout << endl;
}

// stampa la fitness
void Individuo:: StampaFitness(ofstream & fileout){
    fileout << _L << endl;
}


bool Individuo::Check() {

    // controlla che la prima città sia sempre la prima
    if (_path[0] != 1){
        cerr << "Errore: la prima città non è 1, è " << _path[0] << endl;
        exit(1);
    }

    // controlla che _path abbia la lunghezza corretta
    if ((int)_path.size() != _dim){
        cerr << "Errore: la dimensione path è " << _path.size() << " != " << _dim << endl;
        exit(1);
    }

    // controlla che ogni città appaia solo una volta
    vector<int> contatore(_dim + 1, 0);
    for (int i = 0; i < _dim; i++) {
        int c = _path[i];
        if (c < 1 or c > _dim){
            cerr << "Errore: la città " << c <<" è fuori range" << endl;
            exit(1);
        }
        contatore[c]++;
    }
    for (int c = 1; c <= _dim; c++){
        if (contatore[c] != 1){
            cerr << "Errore: la città " << c << " appare " << contatore[c] << " volte" << endl;
            exit(1);
        }
    }

    return true;
}