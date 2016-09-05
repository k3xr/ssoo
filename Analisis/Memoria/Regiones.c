/* Regiones.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira 
 * Versión: 1.1		Fecha: 9 Sep 2009
 * Descripción:
 *	Muestra a qué región pertenecen distintas variables de un programa
 *
 * Uso:
 *	Regiones
 */
#define MYNAME	"Regiones"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_mapa(){
        char mandato[256];
        int mipid;

	printf("\n-------------------------------------------------------------------\n");
	mipid= getpid();
        sprintf(mandato, "pmap %d ", mipid);
        system(mandato);
	printf("-------------------------------------------------------------------\n");
}

int A;		
int B=666;	
int C[4000];
const int D=1000;
int E[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main(int argc, char **argv) { 
	int F;		
	int G[2500];

	/* Se muestra el mapa */
	mostrar_mapa();

	/* Se imprime dirección de variables especificadas en enunciado */
	printf("\n%p\tmain\n%p\tA\n%p\tB\n%p\tC\n%p\tD\n%p\tE\n%p\tF\n%p\tG\n%p\targc\n", main, &A, &B, C,&D, E, &F, G, &argc);

	return 0;
}

	

