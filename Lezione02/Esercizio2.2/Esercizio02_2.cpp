#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
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
            rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "Non riesco ad aprire seed.in" << endl;



    int N=100; //blocchi
    int M=10000; //lanci totali
    int L=M/N; //lanci per blocco
    int P=100; //passi del random walk

// ----- PUNTO 1: reticolo di passo a  ---------------------------------------------

    ofstream stampa1("reticolo.dat");
     
    // ciclo sui passi
    for (int p = 1; p <= P; p++) {

        double acc = 0.0;
        double acc2 = 0.0;

        // ciclo sui blocchi
        for (int i = 0; i < N; i++) {

            double sum = 0.0;

            // lanci nel blocco
            for (int j = 0; j < L; j++) {

                double x = 0.0;
                double y = 0.0;
                double z = 0.0;

                // random walk di p passi
                // può fare avanti-indietro in tutte e 3 le direzioni
                for (int step = 0; step < p; step++) {
                    double a=rnd.Rannyu(0,6);
                    int b=floor(a);
                    if(b==0) x+=1;
                    if(b==1) x-=1;
                    if(b==2) y+=1;
                    if(b==3) y-=1;
                    if(b==4) z+=1;
                    if(b==5) z-=1;
                }

                sum += x*x + y*y + z*z;
            }

            double media=sqrt(sum/L);

            acc += media;
            acc2+= media*media;
        }

        stampa1 << p << "\t" << acc/N << "\t" << error(acc,acc2, N) << endl;
    }


// ----- PUNTO 2: libero in spazio 3D  ----------------------------------------


    ofstream stampa2("continuo.dat");
     
    // ciclo sui passi
    for (int p = 1; p <= P; p++) {

        double acc = 0.0;
        double acc2 = 0.0;

        // ciclo sui blocchi
        for (int i = 0; i < N; i++) {

            double sum = 0.0;

            // lanci nel blocco
            for (int j = 0; j < L; j++) {

                double x = 0.0;
                double y = 0.0;
                double z = 0.0;

                // random walk di p passi
                for (int step = 0; step < p; step++) {
                    double costheta=rnd.Rannyu(-1,1);
                    double phi=rnd.Rannyu(0, 2*M_PI);
                    double sintheta = sqrt(1-costheta*costheta);

                    x+=sintheta*cos(phi);
                    y+=sintheta*sin(phi);
                    z+=costheta;

                }
                sum += x*x + y*y + z*z;
            }
            double media=sqrt(sum/L);

            acc += media;
            acc2+= media*media;
        }

        stampa2 << p << "\t" << acc/N << "\t" << error(acc,acc2, N) << endl;
    }




    return 0;
}
  