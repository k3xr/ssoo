/* Exec_th.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *      Exec_th: Muestra la evolución del mapa de memoria de un proceso
 *      cuando un thread del mismo realiza un exec
 *
 * Uso:
 *      Exec_th
 *
 *      Se debe compilar así: cc -Wall Exec_th.c -o Exec_th -lpthread
 *
 */
#define MYNAME	"Exec_th"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>


/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_estado(char *titulo, int pid_padre, int pid_hijo){
        char mandato[256];

	printf("\n----------- %s: Procesos y threads ---\n", titulo);
        sprintf(mandato, "ps -Lef | head -1");
        system(mandato);
        sprintf(mandato, "ps -Lef | grep %d | grep -v grep", pid_padre);
        system(mandato);
	if (pid_padre) {
		printf("\n----------- %s: Mapa del Padre ------------\n", titulo);
        	sprintf(mandato, "pmap %d", pid_padre);
        	system(mandato);
		printf("-------------------------------------------------------------------\n");
	}
	if (pid_hijo) {
		printf("\n----------- %s: Mapa del Hijo ------------\n", titulo);
        	sprintf(mandato, "pmap %d", pid_hijo);
        	system(mandato);
		printf("-------------------------------------------------------------------\n");
	}
}

static void *funcion_thread(void *arg) {

        /* Se muestra el estado antes del exec en el thread */
	mostrar_estado("Antes del exec en el thread", getppid(), getpid());
	execlp("cat", "cat", NULL);
        return NULL;
}

int main(int argc, char **argv) {
	int pid_hijo;
        pthread_t thid;

	
	mostrar_estado("Antes del fork", getpid(), 0);
	if ((pid_hijo=fork())==0) {
		/* Se muestran el estado antes del exec */
		mostrar_estado("Antes de crear el thread", getppid(), getpid());
        	/* Se crea el thread */
	        pthread_create(&thid, NULL, funcion_thread, NULL);
		pause();
		exit(0);
	}
	/* Se muestran los mapas después del exec */
	sleep(2);
	mostrar_estado("Después del exec", getpid(), pid_hijo);
	kill(pid_hijo, SIGKILL); 
	wait(NULL);
	return 0;
}
