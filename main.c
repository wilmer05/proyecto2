#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"entrada.h"
#include"workers.h"

int main(int argc, char *argv[]){
	dir = (char *)malloc(100);
	strcpy(dir,".");
	validar_inicializar(argc,argv);	
	maestro(dir);

	return 0;
}
