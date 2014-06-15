#include"workers.h"


int cnt=0;

pthread_mutex_t mutex; //mutex para el contador
pthread_t hilo;


//buffer para la escritura final
char buffer[TAM3];

//conteo de tamanio total, archivos y directorios respectivamente
long long tam;
int cnt_arch;
int cnt_dir;
int cnt_proc;


void inic_c(contador *ptr){
	ptr->archivos=0;
	ptr->peso=0LL;
	ptr->directorios=0;
}

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
contador * procesar(char *arch, int *f, int offset){
		struct stat buf;
		char tmp[500];
		stat(arch,&buf);

		contador *res = (contador *)malloc(sizeof (contador));
		inic_c(res);
		contador *temp;

		//si es un archivo escribo su tamanio en el pipe
		if(!(buf.st_mode & S_IFDIR)){

			sprintf(tmp,"%lld %s\n",(long long)buf.st_size,arch);
			write(*(f+offset+3),tmp,strlen(tmp));
			res->peso=(long long)buf.st_size;
			res->archivos=1;
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
				temp=procesar(tmp,f,offset);
				res->peso+=temp->peso;
				res->archivos+=temp->archivos;
				res->directorios+=temp->directorios;
				free(temp);
			}

			//este es un directorio asi que sumo 1 
			(res->directorios)++;
			strcpy(tmp,arch);	
			sprintf(tmp,"%lld %s\n",res->peso,arch);
			write(*(f+offset+3),tmp,strlen(tmp));
			closedir(directorio);

		}
		return res;
}


void maestro(char *d){

	tam = 0LL;
	cnt_arch=0;
	cnt_dir=0;
	cnt_proc=0;

	int fds[6*MAX_PROC]; //aqui iran todos los descriptores de los posibles 
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
			cnt_arch++;
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
			pipe(fds+procs+4);

			//se crea un nuevo proceso
			cnt_proc++;
	
			

			//creo el hijo
			if(!fork()){
				//este hijo no escribira por el pipe procs+1
				close(*(fds+procs+1));
				//ni tampoco leera por el pipe procs+2
				close(*(fds+procs+2));

				close(fds[procs+4]);
				contador *temp;
				char tmp[300];

				//el hijo verifica que directorio debe explorar, cosa que le comunica el papa
				read(*(fds+procs),tmp,300);

				//proceso el directorio o archivo, segun sea el caso
				//la funcion procesar se encargara
				temp=procesar(tmp,fds,procs);
				sprintf(tmp,"%lld %d %d",temp->peso,temp->archivos,temp->directorios);
				write(fds[procs+5],tmp,strlen(tmp));

				kill(getppid(),SIGCHLD);

				//cierro pipes utilizados por el hijo
				close(fds[procs]);
				close(fds[procs+3]);
				close(fds[procs+5]);
				exit(0);
			}
			else{
				//el papa no leera por el pipe numero procs
				close(*(fds+procs));

				//ni escribira por el pipe procs+3
				close(*(fds+procs+3));

				close(fds[procs+5]);
				write(*(fds+procs+1),path2,strlen(path2));
				procs+=6;
			}
		}
		strcpy(path2,path);
	}
	
	int t;
	//espero a que terminen mis procesos
	for(int i=0;i<I;i++) wait(&t);
	
	for(int i=0;i<procs;i+=6){
	
		read(fds[i+2],buffer,TAM3);	
		fprintf(ptr,"%s",buffer);

		//temporales para leer la cantidad total que mi hijo leyo de archivos, dirs, etc
		int temp1;
		int temp2;
		long long temp3;

		read(fds[i+4],buffer,TAM3);
		sscanf(buffer,"%lld %d %d",&temp3,&temp1,&temp2);
		cnt_arch+=temp1;
		cnt_dir+=temp2;
		tam+=temp3;
		
		//cierro los pipes del maestro que aun siguen abiertos
		close(fds[i+1]);
		close(fds[i+2]);
		close(fds[i+4]);
	}
	fprintf(ptr,"%lld %s\n",tam,d);
	fprintf(stdout,"Numero de archivos encontrados: %d\n",cnt_arch);
	fprintf(stdout,"Numero de directorios explorados (incluyendo el principal): %d\n",cnt_dir+1);
	fprintf(stdout,"Numero total de procesos creados (conteo total, no concurrente): %d\n",cnt_proc+1);

	closedir(directorio);
	fclose(ptr);
}
