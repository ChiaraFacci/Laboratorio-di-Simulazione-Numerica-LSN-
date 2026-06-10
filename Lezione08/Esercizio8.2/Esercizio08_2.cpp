#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include "../../GenRandom/random.h"

using namespace std;

// calcola errore per data blocking
double error(double acc, double acc2, int blk);

// Funzione d'onda Psi_T(x)
double Psi(double x, double mu, double sigma);
// densità di probabilità |Psi_T(x)|^2
double Psi2(double x, double mu, double sigma);
// Derivata seconda analitica di Psi_T(x)
double Psi_second(double x, double mu, double sigma);

// Potenziale esterno V(x) = x^4 - 5/2 * x^2
double V(double x);
// Energia locale E_L(x) = H*Psi_T(x) / Psi_T(x)
double En(double x, double mu, double sigma);

// Esegue un passo Metropolis con proposta uniforme in [x-delta, x+delta] 
// campionando punti dalla distribuzione |Psi_T|^2
// Aggiorna x e restituisce true se la mossa è accettata (per monitorare l'acceptance rate)
bool Metro(double & x, double mu, double sigma, double delta, Random & rnd);

// Esegue una simulazione VMC con parametri (mu, sigma) fissati
// Restituisce la stima di <H>_T tramite data blocking su N blocchi e M lanci totali
// Usata internamente dal Simulated Annealing per valutare il costo di ogni configurazione
double VMC(double mu, double sigma, double delta, Random & rnd, int N, int M);



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



// ------- inizio esercizio---------------------------------------

   const double mu_start = 2.;
   const double sigma_start = 1.;
   double delta = 1.1;

   const double delta_mu = 0.15;
   const double delta_sigma = 0.08;
 
   // --- Parametri SA ----------------------------------------------------
   const int M = 10000;
   const int N = 100;

   // range di temperature
   const double T_start = 2.;
   const double T_stop = 0.01;

   const double a = 0.92; // fattore di raffreddamento geometrico
   
   int max_steps = 20; // numero massimo di passi Metropolis per livello di temperatura
 
 
   // --- Stato iniziale ---------------------------------------------------
   double E_now = VMC(mu_start, sigma_start, delta, rnd, N, M);
   double mu_now = mu_start;
   double sigma_now = sigma_start;
 
   double E_best = E_now;
   double mu_best = mu_now;
   double sigma_best = sigma_now;
 
   ofstream sa_out("sa_trajectory.dat");
   ofstream best_out("best_values.dat");
 
   int acc  = 0;
   int step = 0;

   sa_out << setw(6)  << "Step"
            << setw(14) << "Temp"
            << setw(14) << "Mu"
            << setw(14) << "Sigma"
            << setw(14) << "Energy" << endl;

 
   for(double T = T_start; T > T_stop; T *= a){
 
      // Il numero di passi per livello di temperatura cresce man mano che T scende
      // round(T_start/T) aumenta al calare di T, permettendo un'esplorazione più fine vicino al minimo 
      // Si limita a max_steps per evitare tempi eccessivi alle temperature più basse
      // e a un minimo di 1 per sicurezza
      int steps_per_T = min(max_steps, max(1, (int)round(T_start / T)));

      //step a T fissata
      for(int k = 0; k < steps_per_T; k++){
         step++;
 
         double mu_trial = mu_now + rnd.Rannyu(-delta_mu, delta_mu);
         double sigma_trial = sigma_now + rnd.Rannyu(-delta_sigma, delta_sigma);
 
         // I valori negativi vengono riflessi:
         if(sigma_trial < 0.0) sigma_trial = -sigma_trial;
         if(mu_trial < 0.0) mu_trial = -mu_trial;

         double E_trial = VMC(mu_trial, sigma_trial, delta, rnd, N, M);
 
         double dE = E_trial - E_now;
         double prob = exp(-dE / T);
 
         if(rnd.Rannyu() < prob){
            mu_now = mu_trial;
            sigma_now = sigma_trial;
            E_now = E_trial;
            acc++;
         }
 
         if(E_now < E_best){
            E_best = E_now;
            mu_best = mu_now;
            sigma_best = sigma_now;
         }
 
         sa_out << setw(6)  << step
                << setw(14) << T
                << setw(14) << mu_now
                << setw(14) << sigma_now
                << setw(14) << E_now << endl;
      }
 
   }
 
   sa_out.close();

   best_out   << setw(14) << mu_best
            << setw(14) << sigma_best << endl;

   cout << "\nAccettanza totale: " << double(acc)/double(step)*100 << "%"<< endl;
   cout << "Parametri ottimali: mu=" << mu_best << "  sigma=" << sigma_best << endl;
   cout << "Step fatti: step=" << step << endl;

   best_out.close();


// --- stima E con parametri ottimali --------------------------------------------------

   const int M_fin = 20000;
   const int N_fin = 100;
   const int L_fin= int(M_fin/N_fin);
   delta = 2.65; // nuovo delta per avere accettanza al 50% circa

   ofstream en_out("energy_best.dat");
   ofstream psi_out("psi_sample.dat");
   int accepted_fin = 0;

   // termalizzazione prima della run finale
   double x = rnd.Rannyu(-delta, delta);
   for(int t = 0; t < 5000; t++) Metro(x, mu_best, sigma_best, delta, rnd);

   double sum = 0;
   double quad = 0;

   for(int i=0; i<N_fin; i++){
      double accu = 0;

      for(int j=0; j<L_fin; j++){
         if(Metro(x,mu_best,sigma_best,delta,rnd)){accepted_fin++;}     
         accu += En(x, mu_best, sigma_best);
         psi_out << x << endl;
      }
      
      double media = accu/L_fin;

      sum += media;
      quad += media*media;

      en_out   << setw(15) << i+1
               << setw(15) << media
               << setw(15) << sum/double(i+1)
               << setw(15) << error(sum, quad, i+1) << endl;

   
      if(i==N_fin-1){
         cout << endl << "Energia minima:     E=" << sum/N_fin << " +/- " << error(sum, quad, i+1) << endl;
      }

   }

   cout << "Accettanza finale:   " << double(accepted_fin)/double(M_fin)*100 << "%" << endl;
   en_out.close();
   psi_out.close();


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

double VMC(double mu, double sigma, double delta, Random & rnd, int N, int M){
 
   const int L = int(M / N);
 
   double x = rnd.Rannyu(-delta, delta);
 
   double sum  = 0.0;
   double quad = 0.0;
 
   for(int i = 0; i < N; i++){
      double accu = 0.0;
      for(int j = 0; j < L; j++){
         Metro(x, mu, sigma, delta, rnd);
         accu += En(x, mu, sigma);
      }
      double media = accu / L;
      sum  += media;
      quad += media * media;
   }
 
   double E_mean = sum / double(N);
 
   return E_mean;
}