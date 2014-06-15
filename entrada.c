#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"entrada.h"

void ayuda(){
	printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
	printf("-n i indica i niveles de concurrencia\n");
	printf("-d directorio indica el directorio desde donde se calcula el espacio utilizado\n");
	printf("salida es el archivo donde se imprimira todo el espacio utilizado\n");
}

int IsaNumber(int num, char arg)
{
  if (!num) {
    printf("Error en linea de argumentos \n");
    printf("El argumento %c debe ser un entero \n",arg);
    printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
    return (0);
  }
  return (1);
}

void validar_inicializar(int argc, char *argv[]){
  int posicion;
  /***************************************************************
  /* Chequea el numero de argumentos minimo y maximo
  /***************************************************************/
  	if (argc!=2 && argc!=4 && argc!=6) {
    	printf("Error en linea de argumentos \n");
	    printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
	    exit(1);
  	}

  /***************************************************************
  /* Chequea el resto de los parametros
  /*  while ((posicion < argc - 1))
  /*  Con este condicional valido solo las opciones con - (el - 1) 
  /*  evita que se procese el ultimo argumento dentro del while.
  /***************************************************************/

	if(argc==2 && !strcmp(argv[1],"-h")){
		ayuda();
		exit(0);
	}

	I=1;

  	posicion=1;

  	while ((posicion < argc - 1)) {
		if(!strcmp(argv[posicion],"-h")){
			printf("Error en linea de argumentos \n");
	      	printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
	      	exit(1);

		}
    	if(!strcmp(argv[posicion],"-n")) {
      		I = atoi(argv[++posicion]);
      		if (!IsaNumber(I,'i')) {
				exit(1);
      		}
      		posicion = posicion + 1;
      		continue;
    	}

    	if (!strcmp(argv[posicion],"-d")) {
			free(dir);
			dir = argv[++posicion]; //este sera el archivo de donde se empzara a buscar
			int fd = open(dir,O_RDONLY);
			if(fd==-1){
				printf("Directorio inexistente\n");
				exit(1);
			}
			close(fd);
			struct stat buf;
			if(stat(dir,&buf)==-1){
				fprintf(stderr,"El directorio pasado no existe o no se pudo abrir\n.");
				exit(1);
			}
			if(!(buf.st_mode & S_IFDIR)){
				fprintf(stderr,"El directorio pasado no es un directorio\n");
				exit(1);
			}
      		posicion = posicion + 1;
	      	continue;
	    }
	 else {
	    // Caso por defecto 
	     	printf("Error en linea de argumentos \n");
	      	printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
	      	exit(1);
	    }
	}

	struct stat buf;
	if(stat(argv[posicion],&buf)==-1){
		fprintf(stderr,"El archivo de salida pasado no existe o no se pudo abrir\n.");
		exit(1);
	}
	if((buf.st_mode & S_IFDIR)){
		fprintf(stderr,"El archivo de salida pasado es un directorio\n");
		exit(1);
	}

	ptr = fopen(argv[posicion],"w");
	
}
