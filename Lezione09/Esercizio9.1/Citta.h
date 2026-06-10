#ifndef __Citta__
#define __Citta__

#include "../../GenRandom/random.h"
#include <vector>

using namespace std;

class Citta {
    private:
    vector<double> _pos_x; // vector con la pos x delle città
    vector<double> _pos_y; // vector con la pos y delle città
    Random _rnd; // gen di numeri casuali
    int _dim; // numero di città

    public:
    void Initialize_circ(int N); // crea N città disposte random lungo una circonferenza di r=1
    void Initialize_random(int N); // crea N città disposte random in un quadrato L=1
    double GetPosX(int i); // restituisce la pos x della i-esima città
    double GetPosY(int i); // restituisce la pos y della i-esima città
    double Distanza(int i, int j); // calcola distanza tra la città i-esima e j-esima

};

#endif