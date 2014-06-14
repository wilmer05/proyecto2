CC = gcc
HEAD = entrada.h workers.h
OBJ = entrada.o main.o workers.o
OPT = -std=c99
PROG = UsoDisco

all: $(PROG)

$(PROG): $(HEAD) $(OBJ)
	$(CC) $(OBJ) -o $(PROG) $(OPT)

main.o: main.c
	$(CC) -c main.c $(OPT)

workers.o: workers.c workers.h
	$(CC) -c workers.c $(OPT)

entrada.o: entrada.h entrada.c
	$(CC) -c entrada.c $(OPT)

clean:
	rm $(OBJ) $(PROG)
