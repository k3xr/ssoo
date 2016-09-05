/* Thread.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Thread: Muestra la evolución del mapa de memoria de un proceso
 *	cuando se crea un thread
 *
 * Uso:
 *	Thread
 *
 * Compilar:
 *	cc -Wall Thread.c -o Thread -lpthread
 */
#define MYNAME	"Thread"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_mapa(char *titulo){
        char mandato[256];
        int mipid;

	printf("\n----------- %s ------------\n", titulo);
	mipid= getpid();
        sprintf(mandato, "pmap %d", mipid);
        system(mandato);
	printf("-------------------------------------------------------------------\n");
}

static void *funcion_thread(void *arg) {
	/* Se muestra el mapa durante la vida del thread */
	mostrar_mapa("El thread está en ejecución");
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t thid;

	/* Se muestra el mapa antes de crear el thread */
	mostrar_mapa("Antes de crear el thread");
	
	/* Se crea el thread */
	pthread_create(&thid, NULL, funcion_thread, NULL);

	/* Se espera a que termine el thread */
	pthread_join(thid, NULL);

	return 0;
}
