#include<stdio.h>

#ifndef entrada
#define entrada
#define TAM 1000
#define TAM2 1000 //tamanio de cadenas temporales
#define TAM3 1000000
char comando[TAM];
FILE *ptr; //archivo a escribir
char *dir;

int I;

int IsaNumber(int num, char arg);

void validar_inicializar(int argc, char *argv[]);
#endif

#ifndef S_IFDIR
	#define S_IFDIR 0040000
#endif
