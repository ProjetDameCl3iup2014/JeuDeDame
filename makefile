CC=gcc
CCOPT=-g
FLAGS=
prog : main.o sdd.o
	$(CC) $(CCOPT) main.o sdd.o -o jdame $(FLAGS)
main.o : main.c sdd.h
	$(CC) $(CCOPT) -c main.c -o main.o $(FLAGS)
sdd.o : sdd.c sdd.h
	$(CC) $(CCOPT) -c sdd.c -o sdd.o $(FLAGS)

clean :
	rm -f main.o sdd.o jdame 
