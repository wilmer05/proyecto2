#include<stdio.h>

#ifndef entrada
#define entrada
#define TAM 1000
#define S_IFDIR 0040000
char comando[TAM];
FILE *ptr; //archivo a escribir
char *dir;

int I;

int IsaNumber(int num, char arg);

void validar_inicializar(int argc, char *argv[]);
#endif
