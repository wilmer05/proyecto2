#include"workers.h"


int cnt=0;

pthread_mutex_t mutex; //mutex para el contador
pthread_t hilo;

//funcion utilizada por sincronizacion
//para aumentar el contador de procesos concurrentes hasta el momento
void *inc(void *x){
	pthread_mutex_lock(&mutex);
	cnt++;
	pthread_mutex_unlock(&mutex);
}

//igual que la anterior pero de decremento
void *dec(void *x){
	pthread_mutex_lock(&mutex);
	cnt--;
	pthread_mutex_unlock(&mutex);
}

//manejador de la seniales de los hijos, para el proceso maestro
void childHandler(){
	pthread_create( &hilo, NULL, dec,NULL);
	pthread_join(hilo,NULL);
}

//funcion para procesar la exploracion de un archivo o directorio
long long procesar(char *arch, int *f, int offset){
		struct stat buf;
		char tmp[500];
		stat(arch,&buf);

		long long tamanio=0LL;	

		//si es un archivo escribo su tamanio en el pipe
		if(!(buf.st_mode & S_IFDIR)){

			sprintf(tmp,"%lld %s\n",(long long)buf.st_size,arch);
			write(*(f+offset+3),tmp,strlen(tmp));
			tamanio=(long long)buf.st_size;
		}
		else{
			//si es un directorio exploro recursivamente
			DIR *directorio;
			struct dirent *ent;
			directorio=opendir(arch);

			while(ent=readdir(directorio)){
				if(!strcmp(".",ent->d_name) || !strcmp("..",ent->d_name)) continue;

				strcpy(tmp,arch);
				strcat(tmp,"/");
				strcat(tmp,ent->d_name);
				//exploro recursivamente
				tamanio+=procesar(tmp,f,offset);
			}
			strcpy(tmp,arch);	
			sprintf(tmp,"%lld %s\n",tamanio,arch);
			write(*(f+offset+3),tmp,strlen(tmp));
			closedir(directorio);
		}
		return tamanio;
}

//buffer para la escritura final
char buffer[TAM3];


void maestro(char *d){

	int fds[4*MAX_PROC]; //aqui iran todos los descriptores de los posibles 
						 //hijos, en las posiciones pares iran los de lectura
						 //y en los pares iran los de escritura

	int procs =0; //Contador de cuantos hijos se han creado hasta ahora


	//inicializacion del mutex
	pthread_mutex_init(&mutex,NULL);
	int chr;
	DIR *directorio;
	struct dirent *ent;
	char path[TAM2];
	char path2[TAM2];

	//en el path copio a el directorio a explorar
	strcpy(path,d);
	int sz = strlen(d);
	if(path[sz-1]!='/')
		path[sz]='/',path[sz+1]='\0';
	signal(SIGCHLD,childHandler);
	strcpy(path2,path);

	directorio = opendir(dir);
	long long tam = 0LL;

	//busco en todas las entradas del directorio que me mandaron a explorar
	while(ent=readdir(directorio)){
		//si es el mismo, o el papa, continuo
		if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..")) continue;


		struct stat buf;
		strcat(path2,ent->d_name);
		stat(path2,&buf);

		//veo si es un archivo, en ese caso imprimo de una vez su tamanio
		if(!(buf.st_mode & S_IFDIR)){
			fprintf(ptr,"%lld %s\n",(long long)buf.st_size,path2);
			tam+=(long long)buf.st_size;
		}
		//sino, entonces es un directorio y tengo que hacer un fork
		//para que un hijo se encargue de calcular su tamanio
		else{

			//mientras la cantidad de procesos concurrentes sea mayor o igual
			//a la dada, espero
			//cnt se decrementara cuando un hijo mande una senial SIGCHLD
			//que la utilice para indicar que iba a terminar
			while(cnt>=I);

			//creo el hilo de sincronizacion para el incremento del contador de procesos
			//concurrentes

			//creo dos pipes, uno para que el papa lea y otro
			//para que el papa escriba y viceversa para el hijo
			pthread_create( &hilo, NULL, inc,NULL);
			pthread_join(hilo,NULL);
			pipe(fds+procs);
			pipe(fds+procs+2);
	
			//creo el hijo
			if(!fork()){
				//este hijo no escribira por el pipe procs+1
				close(*(fds+procs+1));
				//ni tampoco leera por el pipe procs+2
				close(*(fds+procs+2));
				char tmp[300];
				read(*(fds+procs),tmp,300);
				//proceso el directorio o archivo, segun sea el caso
				//la funcion procesar se encargara
				tam+=procesar(tmp,fds,procs);
				kill(getppid(),SIGCHLD);
				exit(0);
			}
			else{
				//el papa no leera por el pipe numero procs
				close(*(fds+procs));

				//ni escribira por el pipe procs+3
				close(*(fds+procs+3));
				write(*(fds+procs+1),path2,strlen(path2));
				procs+=4;
			}
		}
		strcpy(path2,path);
	}
	
	for(int i=0;i<procs;i+=4){
	
		read(fds[i+2],buffer,TAM3);	
		fprintf(ptr,"%s",buffer);

		//cierro los pipes del maestro que aun siguen abiertos
		close(fds[i+1]);
		close(fds[i+2]);
	}
	fprintf(ptr,"%lld %s\n",tam,d);
	closedir(directorio);
	fclose(ptr);
}
