/* PIPELINE.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Puesta en ejecución de la secuencia de mandatos "ls|wc".
 * Uso:
 *	PIPELINE
 */
#define MYNAME	"PIPELINE"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int pp[2];

	if (pipe(pp) == -1) {
		perror(MYNAME": pipe");
		exit(1);
	}
	switch (fork()) {
	default:		/*Productor */
		close(1);
		dup(pp[1]);
		close(pp[0]);
		close(pp[1]);
		execlp("ls", "ls", 0);
		perror(MYNAME": ls");
		break;
	case 0:			/*Consumidor */
		close(0);
		dup(pp[0]);
		close(pp[0]);
		close(pp[1]);
		execlp("wc", "wc", 0);
		perror(MYNAME": wc");
		break;
	case -1:		/* Error */
		perror(MYNAME": fork");
		break;
	}
	return 1;
}

/*
 * Ejecuta 	ls | wc
 */
