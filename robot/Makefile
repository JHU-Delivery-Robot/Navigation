CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

main: poten_map.o main.o
	$(CC) -o main -lm main.o poten_map.o

main.o: main.c poten_map.h
	$(CC) $(CFLAGS) -c main.c

poten_map.o: poten_map.c poten_map.h
	$(CC) $(CFLAGS) -c poten_map.c

clean:
	rm -f *.o main *~
