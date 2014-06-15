#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/dir.h>
#include"entrada.h"
#include<signal.h>
#include<pthread.h>
#include<sys/mman.h>
#include<sys/wait.h>
#define MAX_PROC 5000

typedef struct{
	long long peso;
	int archivos;
	int directorios;
	} contador;

void inic_c(contador *);

void *inc(void *);

void *dec(void *);

void childHandler();

void maestro(char *);

contador * procesar(char *, int *, int);
