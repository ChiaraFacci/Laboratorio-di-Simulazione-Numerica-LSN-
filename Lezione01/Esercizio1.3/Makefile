CC = g++
CFLAGS = -Wall -O3 --std=c++11
INCLUDE = -I../../GenRandom

# Target principale
Esercizio01_3.exe: Esercizio01_3.o random.o
	$(CC) Esercizio01_3.o random.o -o Esercizio01_3.exe

# Compilazione del file cpp dell'esercizio
Esercizio01_3.o: Esercizio01_3.cpp
	$(CC) -c Esercizio01_3.cpp $(CFLAGS) $(INCLUDE)

# Compilazione del random.cpp nella cartella GenRandom
random.o: ../../GenRandom/random.cpp ../../GenRandom/random.h
	$(CC) -c ../../GenRandom/random.cpp $(CFLAGS) $(INCLUDE)

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f *.o Esercizio01_3.exe