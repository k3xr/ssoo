#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

/* Funcion auxiliar para leer los mandatos */
void leer_mandatos(FILE*, int con_prompt);

/* Funciones a implementar */
void mandato_load  (char* arg);
void mandato_start ();
void mandato_stop  ();
void mandato_sleep (char* arg);
void mandato_resume();
void mandato_echo  (char* arg);
void mandato_finish();

typedef struct plugin_info_st{
		char* banner;
		void(*plugin_loop)();
		void(*plugin_stop)();
		void(*plugin_resume)();
		int(*plugin_finish)();
} plugin_info_t;

#define VERDADERO 1
#define FALSO     0

plugin_info_t* modulo_plugin;
int posicionArray = 0;   		/*Posicion actual para cargar instancias*/
int contadorr = 0; 				/*Contador de instancias arrancadas*/
int procesos = 0; 				
int array[100];					/*guarda pid de hijos arrancados*/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockStop = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int seguir = 0;


/* FUNCION MAIN DEL PROGRAMA CARGADOR */
int main(int argc, char* argv[])
{
    FILE* fich;

	if(argc<2 || !strcmp(argv[1],"--help"))
    {
    	fprintf(stderr,
	      "%s --help: Muestra la ayuda\n"
	      "%s [-t|-p] [fichero_ordenes]\n",argv[0],argv[0]);
      	return 0;
    }

	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);

	/*Comprueba si procesos o threads*/
	if(!strcmp(argv[1],"-p")){
		procesos = FALSO;
    }
    else if(!strcmp(argv[1],"-t")){
		procesos = VERDADERO;
    }

    else{
		fprintf(stderr,
	  		"%s --help: Muestra la ayuda\n"
	  		"%s [-t|-p] [ficher_ordenes]\n",argv[0],argv[0]);
		return 0;
    }
	
	if(argc==3)
    {
    	fich=fopen(argv[2],"r");
      	if(fich==NULL)
		{
	  		fprintf(stderr,
		  		"%s --help: Muestra la ayuda\n"
		  		"%s [-t|-p] [ficher_ordenes]\n",argv[0],argv[0]);
	 		return 0;
		}
	
      	leer_mandatos(fich,FALSO);     /* Cargando fichero de ordenes */
    }
  	else
      	leer_mandatos(stdin,VERDADERO);     /* Modo interactivo */

  	return 0;
}

/* Lee del fichero indicado (puede ser la entrada estandar [stdin])
 *    una secuencia de mandatos. Por cada uno invoca a la funcion correspondiente */

void leer_mandatos(FILE* fich, int prompt)
{
	int salir=0;
  	char linea[1024];
  	char mandato[1024];
  	char argumento[1024];

 	while(!salir)  
    {
    	if(prompt)
			fprintf(stdout,"cargador> ");

      	if(fgets(linea,1024,fich))
		{
	  		bzero(argumento,sizeof(argumento));
	 		sscanf(linea,"%s %[^\n]",mandato,argumento);

			  if     (!strcmp(mandato,"load" ))
				mandato_load(argumento);
			  else if(!strcmp(mandato,"start" ))
				mandato_start();
			  else if(!strcmp(mandato,"stop"  ))
				mandato_stop(argumento);
			  else if(!strcmp(mandato,"sleep" ))
				mandato_sleep(argumento);
			  else if(!strcmp(mandato,"resume"))
				mandato_resume(argumento);
			  else if(!strcmp(mandato,"finish"))
				mandato_finish(argumento);
			  else if(!strcmp(mandato,"echo"  ))
				mandato_echo(argumento);
			  else if(!strcmp(mandato,"quit"  ))
				salir=1; /* Salimos de la linea de mandatos */
			  else
				fprintf(stderr,"Mandato: '%s' no valido\n",mandato);
			}
    }
}

/* funcion de tratamiento de errores*/
void errores(int tipo, char* arg)
{
	switch(tipo){
	case 1:
		fprintf(stderr, "%s: Error(%d), %s\n", arg, 1, "No se puede cargar");
		break;
	case 2:
		fprintf(stderr, "%s: Error(%d), %s\n", arg, 2, "No se encuentra simbolo");
		break;
	case 3:
		fprintf(stderr, "%s: Error(%d), %s\n", arg, 3, "Parametro de mandato no valido");
		break;
	case 4:
		fprintf(stderr, "%s: Error(%d), %s\n", arg, 4, "Finalizacion anomala del plugin");
		break;
	case 5:
		fprintf(stderr, "%s: Error(%d), %s\n", arg, 5, "Error al crear instancia");
		break;
	}
}

/* Tratamiento señal SIGTERM Threads*/
static void alarmaFinishThreads(){
	int n = modulo_plugin->plugin_finish();
  	pthread_exit(n);
}

/* Tratamiento señal SIGUSR1 Threads*/
static void alarmaStopThreads(){	
	modulo_plugin->plugin_stop();
	pthread_mutex_lock(&lockStop);
	seguir=0;
	while(!seguir){
		pthread_cond_wait(&cond, &lockStop);
	}
	pthread_mutex_unlock(&lockStop);
}

/* Tratamiento señal SIGCONT Threads*/
static void alarmaResumeThreads(){
 	modulo_plugin->plugin_resume();
	pthread_mutex_lock(&lockStop);
	seguir=1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lockStop);
}

/* Tratamiento señal SIGTERM Procesos*/
static void alarmaFinish(){
	int n = modulo_plugin->plugin_finish();
  	exit(n);
}

/* Tratamiento señal SIGUSR1 Procesos*/
static void alarmaStop(){	
	modulo_plugin->plugin_stop();
	pause();
}

/* Tratamiento señal SIGCONT Procesos*/
static void alarmaResume(){
 	modulo_plugin->plugin_resume();
}

void *funcThread (){
	
	pthread_mutex_lock(&lock);
	array[posicionArray]=pthread_self();		/* Introducimos pid del hijo en array */
	contadorr++;									/* Incrementamos contador de instancias */
    posicionArray++;							/* Incrementamos la posicion actual del array */
	pthread_mutex_unlock(&lock);	
	
	signal(SIGTERM, alarmaFinishThreads);
	signal(SIGUSR1, alarmaStopThreads);
	signal(SIGCONT, alarmaResumeThreads);
   	sigset_t mask;
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGCONT);
   	while(1){
		sigprocmask(SIG_BLOCK, &mask, NULL); /*Se ignoran SIGUSR1, SIGCONT*/
		modulo_plugin->plugin_loop();
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
  	}
}

/* Funciones que el alumno debe implementar */
void mandato_load (char* arg)
{
	void *handle;
 	plugin_info_t* (*p_info)();
 	handle = dlopen(arg, RTLD_LAZY);	/* Carga lib */
 	if (!handle) {
 		errores(1,arg);
 		exit(1);
 	}
  
 	*(void **) (&p_info) = dlsym(handle,"plugin_info");   /*  */
  	if (!p_info) {
    	errores(2,arg);	
    	exit(2);
  	}
  
  	modulo_plugin = (*p_info)();
  
 	if (dlerror() != NULL){
    	exit(EXIT_FAILURE);
  	}
  	else{
    	fprintf(stderr,"Modulo %s cargado correctamente\n", arg);
  	}
}

void mandato_start ()
{	
	int n_inst = posicionArray;
	int cont = contadorr+1;
	if(!procesos){
		/*Procesos*/
    	int pid = fork();
    	if(pid < 0){
			errores(5,modulo_plugin->banner);
 			exit(5);
		}
    	else if(pid == 0){
			/*Hijo*/
			signal(SIGTERM, alarmaFinish);
			signal(SIGUSR1, alarmaStop);
			signal(SIGCONT, alarmaResume);
  		  	sigset_t mask;
			sigaddset(&mask, SIGUSR1);
			sigaddset(&mask, SIGCONT);
  	    	while(1){
				sigprocmask(SIG_BLOCK, &mask, NULL); /*Se ignoran SIGUSR1, SIGCONT*/
				modulo_plugin->plugin_loop();
				sigprocmask(SIG_UNBLOCK, &mask, NULL);
  	    	}
  		}
    	else{
			/*Padre*/
			array[posicionArray]=pid;		/* Introducimos pid del hijo en array */
			contadorr++;						/* Incrementamos contador de instancias */
    		posicionArray++;				/* Incrementamos la posicion actual del array */
   		}
  	}
  	else{ 
		/*Threads*/
		pthread_attr_t attr;
		pthread_t thid;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
		if((pthread_create(&thid,&attr,&funcThread, NULL))!=0){
			errores(5,modulo_plugin->banner);
 			exit(5);
		}
		
	}
    fprintf(stderr,"Modulo iniciado: %d\n",n_inst);
    fprintf(stderr,"Numero de modulos iniciados: %d\n",cont);
}

void mandato_stop  (char* arg)
{
	int nuI = 0;		/* Por defecto instancia 0 */
  	if(arg != NULL)
    	nuI = strtol(arg, NULL, 10);    
 
	kill(array[nuI],SIGUSR1); 
	fprintf(stderr,"Modulo detenido: %d\n",nuI);
}

void mandato_sleep (char* arg)
{
    int tiempo = strtol(arg, NULL, 10);
	if(tiempo==0){
		errores(3,arg);
	}
	struct timespec times;
	times.tv_sec = tiempo;
 	times.tv_nsec = 0; 
	nanosleep(&times, NULL);
		
}

void mandato_resume(char* arg)
{
	int nuI = 0;
  	if(arg != NULL)
    	nuI = strtol(arg, NULL, 10); 
 
	kill(array[nuI],SIGCONT); 
    fprintf(stderr,"Modulo reiniciado: %d\n", nuI);
}

void mandato_echo  (char* arg)
{
    fprintf(stderr,"%s\n",arg);
}

void mandato_finish(char* arg)
{
	int nuI = 0;
 	if(arg != NULL){
		nuI = strtol(arg, NULL, 10);
	}

	if(!procesos){
		kill(array[nuI],SIGTERM); 
		contadorr--;
	  	int status;
		/*wait(&status);*/
	  	waitpid(array[nuI], &status, WUNTRACED | WCONTINUED );
	  	/*if(WIFEXITED(status)) por alguna razon esto mete un delay raro*/
		fprintf(stderr,"Modulo finalizado: %d (%d)\n",nuI, WEXITSTATUS(status));
   	}
	else{
		pthread_kill(array[nuI],SIGTERM);
		char *ptr;
		pthread_join(array[nuI], (void**)&ptr);
		contadorr--;
		
		fprintf(stderr,"Modulo finalizado: %d (%d)\n",nuI, ptr);
	}
}

