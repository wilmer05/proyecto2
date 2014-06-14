#include"workers.h"

void maestro(char *d){

	int chr;
	DIR *directorio;
	struct dirent *ent;
	char path[TAM2];
	strcpy(path,d);
	int sz = strlen(d);
	if(path[sz-1]!='/')
		path[sz]='/',path[sz+1]='\0';
	printf("%s\n",path);
	directorio = opendir(dir);
	while(ent=readdir(directorio)){
		if(!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..")) continue;


	}

	closedir(directorio);
}
