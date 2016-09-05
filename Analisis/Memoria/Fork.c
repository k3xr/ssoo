/* Fork.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *      Fork: Muestra la evolución del mapa de memoria de un proceso
 *      y el de su proceso hijo
 *
 * Uso:
 *      Fork
 *
 */
#define MYNAME	"Fork"


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_mapa(char *titulo){
        char mandato[256];
        int mipid;

	mipid= getpid();
	printf("\n----------- Proceso %d: %s ------------\n", mipid, titulo);
        sprintf(mandato, "pmap %d", mipid);
        system(mandato);
	printf("-------------------------------------------------------------------\n");
}

int main(int argc, char **argv) {
	int id_pr;
	char *p;

	/* Se muestra el mapa antes del fork */
	mostrar_mapa("Antes del fork");
	
	id_pr=fork();

	if (!id_pr) {
		p=malloc(10000);
		sleep(1);
	}

	/* Ambos procesos muestran el mapa después del fork */
	mostrar_mapa("Después del fork");

	if (id_pr)
		wait(NULL);

	return 0;
}
