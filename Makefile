CC = gcc
HEAD = entrada.h 
OBJ = entrada.o main.o
OPT = -std=c99
PROG = UsoDisco

all: $(PROG)

$(PROG): $(HEAD) $(OBJ)
	$(CC) $(OBJ) -o $(PROG) $(OPT) -lpthread

main.o: main.c
	$(CC) -c main.c $(OPT) -lpthread


entrada.o: entrada.h entrada.c
	$(CC) -c entrada.c $(OPT) -lpthread

clean:
	rm $(OBJ) $(PROG)
