CC = mpicxx
CFLAGS = -O3 -std=c++17
NP = 3

all: Esercizio10_2.exe

Esercizio10_2.exe : Citta.o Esercizio10_2.o Individuo.o Popolazione.o random.o
	$(CC) Citta.o Individuo.o Popolazione.o random.o Esercizio10_2.o -o Esercizio10_2.exe

Esercizio10_2.o : Esercizio10_2.cpp Citta.h Individuo.h Popolazione.h
	$(CC) -c Esercizio10_2.cpp -o Esercizio10_2.o $(CFLAGS)

Citta.o : Citta.cpp Citta.h ../../GenRandom/random.h Individuo.h
	$(CC) -c Citta.cpp -o Citta.o $(CFLAGS)

Individuo.o : Individuo.cpp Individuo.h
	$(CC) -c Individuo.cpp -o Individuo.o $(CFLAGS)

Popolazione.o : Popolazione.cpp Popolazione.h Individuo.h Citta.h
	$(CC) -c Popolazione.cpp -o Popolazione.o $(CFLAGS)

random.o : ../../GenRandom/random.cpp ../../GenRandom/random.h
	$(CC) -c ../../GenRandom/random.cpp -o random.o $(CFLAGS)

run : Esercizio10_2.exe
	mpirun -np $(NP) --use-hwthread-cpus ./Esercizio10_2.exe

clean :
	rm *.o Esercizio10_2.exe
