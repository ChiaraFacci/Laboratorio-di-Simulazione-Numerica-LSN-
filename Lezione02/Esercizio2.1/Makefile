CC = g++
CFLAGS = -Wall -O3 --std=c++11
INCLUDE = -I../../GenRandom

# Target principale
Esercizio02_1.exe: Esercizio02_1.o random.o
	$(CC) Esercizio02_1.o random.o -o Esercizio02_1.exe

# Compilazione del file cpp dell'esercizio
Esercizio02_1.o: Esercizio02_1.cpp
	$(CC) -c Esercizio02_1.cpp $(CFLAGS) $(INCLUDE)

# Compilazione del random.cpp nella cartella GenRandom
random.o: ../../GenRandom/random.cpp ../../GenRandom/random.h
	$(CC) -c ../../GenRandom/random.cpp $(CFLAGS) $(INCLUDE)

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f *.o Esercizio02_1.exe