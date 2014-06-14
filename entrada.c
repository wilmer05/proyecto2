#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
	int num_N,num_I,num_P;	
  /***************************************************************
  /* Chequea el numero de argumentos minimo y maximo
  /***************************************************************/
	P=N=1;
  	if (argc!=2 && argc!=4 && argc!=6) {
    	printf("Error en linea de argumentos \n");
	    printf("USO: argumentos [-h] | [-n i] [-d directorio] salida \n");
	    exit(1);
  	}

  /***************************************************************
  /* Chequea el resto de los parametros
  /*  while ((posicion < argc - 2))
  /*  Con este condicional valido solo las opciones con - (el - 2) 
  /*  evita que se procesen los dos ultimos argumentos dentro del while.
  /***************************************************************/

	if(argc==2 && !strcmp(argv[1],"-h")){
		ayuda();
		exit(0);
	}


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
			dir = argv[++posicion];

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

	//el archivo de donde se explorara todo es el siguiente argumento`
	printf("%s",argv[posicion]);
	
}
