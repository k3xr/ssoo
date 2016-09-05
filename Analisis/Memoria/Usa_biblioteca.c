/* Usa_biblioteca.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Programa que usa la función coseno. Se pretende enlazarlo de forma
 *	estática y dinámica para comparar el mapa resultante.
 *
 * Uso:
 *	Usa_biblioteca
 *
 * Compilación con enlace estático:
 *	gcc -Wall -o Usa_biblioteca_est Usa_biblioteca.c -static -lm
 *
 * Compilación con enlace dinámico:
 *	gcc -Wall -o Usa_biblioteca_din Usa_biblioteca.c -lm
 *
 */
#define MYNAME	"Usa_biblioteca"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

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

int main(int argc, char **argv) {
	double valor;

	if (argc!=2) {
		fprintf(stderr, "Uso: %s argumento_coseno\n", MYNAME);
		exit(1);
	}

	/* Se muestra el mapa */
	mostrar_mapa("Mapa del proceso");
	
	valor=atof(argv[1]);
	printf("Coseno(%f)=%f\n", valor, cos(valor));

	return 0;
}
