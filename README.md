# Laboratorio-di-Simulazione-Numerica-LSN-

In questa repository sono raccolti tutti gli esercizi svolti durante il corso di **Laboratorio di Simulazione Numerica (LSN)**. Ogni esercitazione è organizzata in una cartella dedicata, contenente il codice sorgente in C++, i notebook Jupyter per l'analisi dei risultati ed eventuali file di supporto.

## Struttura del repository
```plaintext
Laboratorio-di-Simulazione-Numerica-LSN-
├── GenRandom/
├── Lezione01/
├── Lezione02/
├── Lezione03/
├── Lezione04/
├── Lezione06/
├── Lezione07/
├── Lezione08/
├── Lezione09/
├── Lezione10/
├── Lezione11/
├── Lezione12/
└── README.md
```
* **GenRandom/**: contiene il generatore di numeri casuali e i file necessari al suo funzionamento. Viene utilizzato nelle lezioni 1, 2, 3, 8, 9 e 10.
* **LezioneXX/**: contiene le sottocartelle relative ai singoli esercizi EsercizioXX.Y/ e il notebook Jupyter con l'analisi dei risultati.
* **README.md**: descrive la struttura e l'organizzazione della repository.

## Argomenti trattati
* Lezione01: metodi Monte Carlo e data blocking
* Lezione02: importance sampling e random walk
* Lezione03: integrazione stocastica e valutazione di opzioni finanziarie
* Lezione04: dinamica molecolare
* Lezione06: modello di Ising
* Lezione07: dinamica molecolare e tail corrections
* Lezione08: Monte Carlo Variazionale e Simulated Annealing
* Lezione09: problema del commesso viaggiatore e algoritmo genetico
* Lezione10: algoritmo genetico e tecniche di parallelizzazione
* Lezione11: reti neurali
* Lezione12: reti neurali profonde e reti neurali convoluzionali

## Compilazione ed esecuzione
Per eseguire un esercizio:
* Entrare nella cartella relativa all'esercizio desiderato.
* Compilare il codice tramite: "make".
* Eseguire il programma con: "./EsercizioXX_Y.exe", dove XX indica il numero della lezione e Y il numero dell'esercizio.
 
### Eccezioni e comandi aggiuntivi
* Negli esercizi **6.1**, **7.1** e **7.3**, il comando "make copia" permette di utilizzare la configurazione finale generata come nuova configurazione iniziale.
* Nell'esercizio **10.2** è possibile eseguire direttamente il programma tramite: "make run"
* Le **lezioni 11 e 12** sono dedicate esclusivamente all'analisi dei dati mediante notebook Jupyter e non contengono codice C++ da compilare o eseguire.
