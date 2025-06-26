CC = gcc
CFLAGS = -Wall -O2

OBJ1 = tp4_parte1.o algoritmo_pd.o algoritmo_shiftand.o utils.o
OBJ2 = tp4_parte2.o algoritmo_bmh.o huffman.o utils.o utils_simbolos.o

all: tp4_parte1 tp4_parte2

tp4_parte1: $(OBJ1)
	$(CC) $(CFLAGS) -o tp4_parte1 $(OBJ1)

tp4_parte2: $(OBJ2)
	$(CC) $(CFLAGS) -o tp4_parte2 $(OBJ2)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o tp4_parte1 tp4_parte2
