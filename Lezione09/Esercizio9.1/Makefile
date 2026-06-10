CC = g++
CFLAGS = -O3 --std=c++11 -std=c++17
all: Esercizio09_1.exe
Esercizio09_1.exe : Citta.o Esercizio09_1.o Individuo.o Popolazione.o random.o
	$(CC) Citta.o Individuo.o Popolazione.o random.o Esercizio09_1.o -o Esercizio09_1.exe
Esercizio09_1.o : Esercizio09_1.cpp Citta.h Individuo.h Popolazione.h
	$(CC) -c Esercizio09_1.cpp -o Esercizio09_1.o $(CFLAGS)
Citta.o : Citta.cpp Citta.h ../../GenRandom/random.h Individuo.h
	$(CC) -c Citta.cpp -o Citta.o $(CFLAGS)
Individuo.o : Individuo.cpp Individuo.h
	$(CC) -c Individuo.cpp -o Individuo.o $(CFLAGS)
Popolazione.o : Popolazione.cpp Popolazione.h Individuo.h Citta.h
	$(CC) -c Popolazione.cpp -o Popolazione.o $(CFLAGS)
random.o : ../../GenRandom/random.cpp ../../GenRandom/random.h
	$(CC) -c ../../GenRandom/random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o Esercizio09_1.exe
