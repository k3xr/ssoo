/* Heap.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Heap
 *
 * Uso:
 *	Muestra la evolución del heap según ejecuta el programa.
 */
#define MYNAME	"Heap"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

/* Función de prueba */
static void funcion(void) {
        char vector[100000];

        vector[0]=0;

        /* Se muestra el mapa dentro de la función */
        mostrar_mapa("Desde dentro de la función");
}


int main(int argc, char **argv) 
{

        
	char *vector;

	/* Se muestra el mapa antes de llamar a la función */
        mostrar_mapa("Antes de llamar a función");

        funcion();

        /* Se muestra el mapa después de llamar a la función */
        mostrar_mapa("Después de llamar a función");

	/* Se muestra el mapa antes de llamar a malloc */
	mostrar_mapa("Antes de llamar a malloc");
	

	vector=malloc(4000);

	/* Se muestra el mapa después de llamar a malloc */
	mostrar_mapa("Después de llamar a malloc");

	free(vector);
	
	/* Se muestra el mapa después de llamar a free */
	mostrar_mapa("Después de llamar a free");

	vector=malloc(10000);
	

	/* Se muestra el mapa después de llamar a malloc por segunda vez */
	mostrar_mapa("Después de llamar a malloc por segunda vez");

	return 0;
}

	

