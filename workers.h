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

#define MAX_PROC 5000

void *inc(void *);

void *dec(void *);

void childHandler();

void maestro(char *);

long long procesar(char *, int *, int);
