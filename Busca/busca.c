#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

pid_t pid;
char* argV0;
int flagPipe;

char* ruta(char* cwd, char* entradaName)
{
	/*Devuelve la ruta completa de la entrada*/
	char* ruta;
	ruta =(char*)strdup(cwd);
	if(cwd[(strlen(cwd)-1)] == '/'){
		strcat(ruta, entradaName);
	}
	else{
		strcat(ruta, "/");
		strcat(ruta, entradaName);
	}
	return(ruta);
}

void errores(int tipo)
{
	switch(tipo){
	case 1:
		fprintf(stderr, "%s: Error(%d), %s\n", argV0, 1, "Opcion no valida");
		break;
	case 2:
		fprintf(stderr, "%s: Error(%d), %s\n", argV0, 2, "No se puede leer el directorio");
		break;
	case 3:
		fprintf(stderr, "%s: Error(%d), %s\n", argV0, 3, "No se puede leer el archivo");
		break;
	case 4:
		fprintf(stderr, "%s: Error(%d), %s\n", argV0, 4, "No se puede ejecutar el mandato");
		break;
	case 5:
		fprintf(stderr, "%s: Error(%d), %s\n", argV0, 5, "Timeout");
		break;
	}
}

void funPrint(char* cwd, struct dirent *entrada)
{
	/*-print*/
	char *rutaPrint;
	rutaPrint=ruta(cwd,entrada->d_name);
	fprintf(stdout, "%s\n",rutaPrint);
}

void alarma()
{
	/*Timeout*/
	errores(5);
	kill(pid,SIGKILL);
}

void funExec(int argc,char* argv[], struct dirent *entrada)
{
	/*-exec*/
	int status;
	char* mandato[argc-3];
	struct sigaction sig;
	int buscatime;
	int i; 
	int j=0;

	int pidf=fork();
	if(pidf>0){
		/*padre*/
		sig.sa_handler=alarma;
		sig.sa_flags=0;
		sigaction(SIGALRM,&sig,NULL);
		char* buscatimeChar=getenv("BUSCATIME");
		if(buscatimeChar != NULL){
			buscatime =(atoi(buscatimeChar));
			alarm(buscatime);
		}
		else{
			/*BUSCATIME no existe; 0 por defecto*/
			alarm(0);
		}
		/*Espera hijos*/
		wait(&status);
	}	
	else if(pidf==0){
		/*hijo*/
		for (i=5;i<argc;i++){
			mandato[j]=argv[i];
			j++;
		}
		mandato[argc-5]=entrada->d_name;
		mandato[argc-4]=NULL;
		execvp(mandato[0],mandato);
		/*Execvp falla*/		
		errores(4);
		return;
	}
	else{
		/*fork falla*/
		perror("fork");
		exit(-1);
	}
}

void funPipe(int argc,char* argv[], struct dirent *entrada)
{	
	/*-pipe*/
	char* mandato[argc-4];
	int i; 
	int j=0;
	int fd[2];
	pipe(fd);
	if(flagPipe==0){
		int pidf=fork();
		if(pidf>0){
			/*padre*/
			close(1);
			close(fd[0]);
			dup(fd[1]);

			write(1, entrada->d_name, sizeof(entrada->d_name));
		}	
		else if(pidf==0){
			/*hijo*/
			for (i=5;i<argc;i++){
				mandato[j]=argv[i];
				j++;
			}
			mandato[argc-4]=NULL;
			close(0);
			close(fd[1]);
			dup(fd[0]);
			execvp(mandato[0],mandato);
			errores(4);
			return;
		}
		else{
			/*fork falla*/
			perror("fork");
			exit(-1);
		}
	}
	else if(flagPipe<0){
		close(1);
		close(fd[1]);
		return;
	}
	else{
		write(1, entrada->d_name, sizeof(entrada->d_name));	
	}
	flagPipe=1;
}

void seleccionAccion(int argc,char* argv[], struct dirent *entrada)
{
	/* Excluye entradas que empiezan por '.' */
	if(!((entrada->d_name)[0]=='.')){
		if(!strcmp(argv[4], "-print")){
			funPrint(argv[1], entrada);
		}
		else if(!strcmp(argv[4], "-exec")){
			funExec(argc, argv, entrada);
		}
		else if(!strcmp(argv[4], "-pipe")){
			funPipe(argc, argv, entrada);
		}
		else{
			errores(1);
			exit(1);
		}
	}
}

/* FUNCION MAIN DEL PROGRAMA BUSCA */
int main(int argc, char* argv[])
{
	if(argc<2 || !strcmp(argv[1],"-h"))
	{
		fprintf(stdout,
				"busca -h: Muestra la ayuda\n"
				"busca dir [-n|-t|-p|-c|-C] argumento [-R] "
				"[-print|-pipe|-exec] [mandato argumentos..]\n");
		return 0;
	}

	argV0=argv[0];

	if(argc<5){
		errores(1);
		exit(1);
	}

	/* struct stat datos; */
	DIR *dir;
	/*abro directorio*/
	dir = opendir(argv[1]);

	if (dir == NULL) {
		errores(2);
		exit(2);
	}

	/*Cambio directorio*/
	if(chdir(argv[1]) < 0) {
		errores(2);
		exit(2);
	}
	char cwd[256];
	if(getcwd(cwd, sizeof(cwd)) == NULL){
		errores(2);
		exit(2);
	}
	struct dirent *entrada;

	int opcion = 0;
	flagPipe =0;

	if (!strcmp(argv[2], "-t")) {
		opcion = 1;
	}
	if (!strcmp(argv[2], "-n")) {
		opcion = 2;
	}
	if (!strcmp(argv[2], "-p")) {
		opcion = 3;
	}
	if (!strcmp(argv[2], "-c")) {
		opcion = 4;
	}
	if (!strcmp(argv[2], "-C")) {
		opcion = 5;
	}
	switch (opcion) 
	{
	case 1:
	{
		struct stat datos;
		if (!strcmp(argv[3], "f")){
			/*fichero*/
			while ((entrada = readdir(dir)) != NULL) {
				if(stat(entrada->d_name,&datos) < 0) {
					continue;
				}

				if (S_ISREG(datos.st_mode)) {
					seleccionAccion(argc, argv, entrada);
				}
			}
		}
		else if (!strcmp(argv[3], "d")){
			/*directorio*/
			while ((entrada = readdir(dir)) != NULL) {
				if(stat(entrada->d_name,&datos) < 0) {
					continue;
				}
				if (S_ISDIR(datos.st_mode)) {
					seleccionAccion(argc, argv, entrada);
				}
			}
		}
		else{
			errores(1);
			exit(1);
		}
	}
	break;
	case 2:
	{
		while ((entrada = readdir(dir)) != NULL) {
			if(strstr(entrada->d_name, argv[3]) != NULL) {
				seleccionAccion(argc, argv, entrada);
			}
		}
	}
	break;
	case 3:
	{
		struct stat datos;
		if (!strcmp(argv[3], "x")){
			while ((entrada = readdir(dir)) != NULL){
				if(stat(entrada->d_name,&datos) < 0){
					continue;
				}

				if (datos.st_uid==geteuid()) {
					if(datos.st_mode & S_IXUSR){
						/* Permiso de ejecución de usuario */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else if(datos.st_gid==getegid()){
					if(datos.st_mode & S_IXGRP){
						/* Permiso de ejecución de grupo */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else{
					if(datos.st_mode & S_IXOTH){
						/* Permiso de ejecución de otros */
						seleccionAccion(argc, argv, entrada);
					}
				}
			}
		}

		else if (!strcmp(argv[3], "r")){
			while ((entrada = readdir(dir)) != NULL) {
				if(stat(entrada->d_name,&datos) < 0){
					continue;
				}

				if (datos.st_uid==geteuid()) {
					if(datos.st_mode & S_IRUSR){
						/* Permiso de lectura de usuario */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else if(datos.st_gid==getegid()){
					if(datos.st_mode & S_IRGRP){
						/* Permiso de lectura de grupo */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else{
					if(datos.st_mode & S_IROTH){
						/* Permiso de lectura de otros */
						seleccionAccion(argc, argv, entrada);
					}
				}
			}
		}
		else if (!strcmp(argv[3], "w")){
			while ((entrada = readdir(dir)) != NULL) {
				if(stat(entrada->d_name,&datos) < 0){
					continue;
				}

				if (datos.st_uid==geteuid()) {
					if(datos.st_mode & S_IWUSR){
						/* Permiso de escritura de usuario */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else if(datos.st_gid==getegid()){
					if(datos.st_mode & S_IWGRP){
						/* Permiso de escritura de grupo */
						seleccionAccion(argc, argv, entrada);
					}
				}
				else{
					if(datos.st_mode & S_IWOTH){
						/* Permiso de escritura de otros */
						seleccionAccion(argc, argv, entrada);
					}
				}
			}
		}
		else{
			errores(1);
			exit(1);
		}
	}
	break;
	case 4:
	{
		struct stat datos;
		while ((entrada = readdir(dir)) != NULL) {
			if(stat(entrada->d_name,&datos) < 0) {
				continue;
			}
			if (S_ISREG(datos.st_mode)) {
				FILE *fp=fopen(entrada->d_name,"r");
				if (fp == NULL){
					errores(3);
					continue;
				}
				char tmp[512]={0x0};
				int flag=0;
				while((flag == 0) && (fgets(tmp, sizeof(tmp),fp) != NULL))
				{
					if (strstr(tmp, argv[3])){
						seleccionAccion(argc, argv, entrada);
						flag=1;
					}
				}
				if(fp!=NULL) fclose(fp);
			}
		}
	}
	break;
	case 5:
	{
		struct stat datos;
		void *map;

		while ((entrada = readdir(dir)) != NULL) {
			if(stat(entrada->d_name,&datos) < 0) {
				continue;
			}
			if (S_ISREG(datos.st_mode)) {
				int fp=open(entrada->d_name,O_RDONLY);
				if (fp < 0){
					errores(3);
					continue;
				}
				else{
					/*Proyecta en memoria*/
					map=mmap(NULL,datos.st_size,PROT_READ,MAP_SHARED,fp,0);
					if(map==MAP_FAILED){
						continue;
					}
					else{
						char* cadMap = (char *)map;
						if((strstr(cadMap,argv[3]))!=NULL){
							seleccionAccion(argc, argv, entrada);
						}
						munmap(map,datos.st_size);
						close(fp);
					}
				}
			}
		}
	}
	break;
	default:
	{
		errores(1);
		exit(1);
	}

	}/*end switch*/
	if(flagPipe){
		flagPipe=-1;
		funPipe(argc,argv, NULL);
	}
	closedir(dir);
	return 0;

}/*end main*/
