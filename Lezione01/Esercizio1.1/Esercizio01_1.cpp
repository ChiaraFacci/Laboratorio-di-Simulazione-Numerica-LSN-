#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "../../GenRandom/random.h"

double error(double acc, double acc2, int blk){
  if(blk <= 1) return 0.0;
  else return sqrt( fabs(acc2/double(blk) - pow( acc/double(blk) ,2) )/double(blk) );
}

using namespace std;

int main(){

   int M=10000; // numero totale di lanci
   int N=100; // numero di blocchi
   int L= int(M/N); // lanci per blocco
    
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



// ------ PUNTI 1 e 2 --------------------------------------------------------

   ofstream stampa1("dati1.txt");
   ofstream stampa2("dati2.txt");

   double somma1=0;     double somma2=0;
   double quad1=0;      double quad2=0;
   
   // data blocking
   for(int j=0; j<N; j++){   
      double accu1=0; double accu2=0;

      for(int i=0; i<L; i++){
         double n=rnd.Rannyu();
         accu1 += n; 
         accu2 += pow(n-0.5,2);
      }
   
      double media1 = accu1 / L;
      double media2 = accu2 / L;

      somma1 += media1;
      somma2 += media2;
      quad1  += pow(media1, 2);
      quad2  += pow(media2, 2);

      stampa1 << somma1/(j+1) << "\t" << error(somma1, quad1, j+1) << endl;
      stampa2 << somma2/(j+1) << "\t" << error(somma2, quad2, j+1) << endl;

   }   

   stampa1.close(); 
   stampa2.close();

   // ------------ PUNTO 3 ------------------------------------------------------------

   ofstream chiquadro("datichi.txt");

   int nRipetizioni = 100;   // numero di volte che ripetiamo il test chi-quadro
   int nBin = 100;           // numero di sottointervalli in cui dividiamo [0,1]
   int nLanci = 10000;       // numeri casuali generati per ogni realizzazione del test
   double atteso = (double)nLanci / nBin;  // valore atteso per ogni bin: E = n/M = 100


   // loop sulle 100 ripetizioni del test
   for(int j = 0; j < nRipetizioni; j++){  
      
      vector<double> conteggi(nBin, 0.0);  // vettore dei conteggi nizializzato a 0

      // generazione degli nLanci numeri casuali e riempimento dei bin
      for(int i = 0; i < nLanci; i++){

         double n = rnd.Rannyu();
         int x = min((int)(n * nBin), nBin - 1);  // indice del bin corrispondente
         conteggi[x]++;

      }

      // calcolo della statistica chi-quadro per questa realizzazione
      double chiq = 0.0;
      for(int i = 0; i < nBin; i++){
         chiq += pow(conteggi[i] - atteso, 2) / atteso;  // somma (O_i - E_i)^2 / E_i
      }

      chiquadro << chiq << endl;
   }
   chiquadro.close();
      
   return 0;
}