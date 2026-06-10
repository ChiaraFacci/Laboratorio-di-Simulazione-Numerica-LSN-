#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include "../../GenRandom/random.h"

// calcola errore per data blocking
double error(double acc, double acc2, int blk);

// Funzione d'onda Psi_T(x)
double Psi(double x, double mu, double sigma);

// densità di probabilità |Psi_T(x)|^2
double Psi2(double x, double mu, double sigma);

// Derivata seconda analitica di Psi_T(x)
double Psi_second(double x, double mu, double sigma);

// Esegue un passo Metropolis con proposta uniforme in [x-delta, x+delta] 
// campionando punti dalla distribuzione |Psi_T|^2 
// Aggiorna x e restituisce true se la mossa è accettata (per monitorare l'acceptance rate)
bool Metro(double & x, double mu, double sigma, double delta, Random & rnd);

// Potenziale esterno V(x) = x^4 - 5/2 * x^2
double V(double x);

// Energia locale E_L(x) = H*Psi_T(x) / Psi_T(x)
double En(double x, double mu, double sigma);

using namespace std;

int main(){
   
   // per far funzione il generatore di numeri casuali
   Random rnd;

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



// inizio esercizio //////////////////////////////////////////////


   const int M=10000;
   const int N=100;
   const int L= int(M/N);
   
   const double mu = 1.1;
   const double sigma = 0.5;
   const double delta=1.1; // ampiezza della proposta uniforme, scelta per acceptance circa 50%

   // Punto di partenza casuale, poi termalizzazione con 1000 passi Metropolis
   double x = rnd.Rannyu(-delta, delta);
   for(int t = 0; t < 1000; t++) Metro(x, mu, sigma, delta, rnd);
   ofstream stampa("energia.dat");

   double sum = 0;
   double quad = 0;


   // Data blocking: M misure totali divise in N blocchi da L passi ciascuno
   // Per ogni blocco si calcola la media locale di E_L, poi si accumula
   // sum e sum^2 per stimare media progressiva ed errore statistico

   int acc=0; //acceptance
   for(int i=0; i<N; i++){
      double accu = 0;

      for(int j=0; j<L; j++){
         if(Metro(x,mu,sigma,delta,rnd)) acc++;    
         accu += En(x, mu, sigma);
      }
      
      double media = accu/L;

      sum += media;
      quad += media*media;

      stampa   << setw(15) << i+1
               << setw(15) << media
               << setw(15) << sum/double(i+1)
               << setw(15) << error(sum, quad, i+1) << endl;


   }

   stampa.close();
   cout << "Acceptance rate: " << double(acc)/(N*L)*100 <<"%"<< endl;
   
   return 0;
}

double error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

double Psi(double x, double mu, double sigma){
    return exp(-pow(x-mu,2)/(2.0*sigma*sigma)) + exp(-pow(x+mu,2)/(2.0*sigma*sigma));
}

double Psi2(double x, double mu, double sigma){
    return fabs(Psi(x, mu, sigma)*Psi(x, mu, sigma));
}

double Psi_second(double x, double mu, double sigma){

    double Psi_meno = exp(-pow(x-mu,2)/(2.0*sigma*sigma));
    double Psi_piu = exp(-pow(x+mu,2)/(2.0*sigma*sigma));

    double f1 = (pow(x-mu,2)-sigma*sigma)*Psi_meno/(pow(sigma,4));
    double f2 = (pow(x+mu,2)-sigma*sigma)*Psi_piu/(pow(sigma,4));

    return f1+f2;
}

double V(double x){
   return pow(x,4)-2.5*x*x;
}

double En(double x, double mu, double sigma){
   return -0.5*Psi_second(x, mu, sigma) / Psi(x, mu, sigma)+V(x);
}

bool Metro(double & x, double mu, double sigma, double delta, Random & rnd) {
   bool accept = false;
    double x_prop = x + rnd.Rannyu(-delta, delta);
    double ratio  = Psi2(x_prop, mu, sigma) / Psi2(x, mu, sigma);

    if (rnd.Rannyu() < min(1.0, ratio)) {
        x = x_prop;
        accept = true;
    }
    return accept;
}

