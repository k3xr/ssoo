/* Concurr_prc.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 21 Sep 2004
 * Descripción:
 *	El programa demuestra la concurrencia entre procesos.
 *	Se crean tantos hijos como argumentos de invocación, y cada
 *	hijo consume cierta CPU e imprime el correspondiente argumento.
 *
 * Uso:
 *	Concurr_prc A B C D E F ...
 */
#define MYNAME	"Concurr_prc"


#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * echo(void * str)
{
	int cnt;
	int consumo;

	for (cnt=0; cnt<10000; cnt++) {
		for (consumo=0; consumo<10000; consumo++)
			/* Se consume CPU haciendo lo que sea... */
			continue;
		fprintf(stderr, "%s", (char *)str);
//NO		sched_yield();		/* Ceder la CPU */
	}
	return NULL;
}

int main(int argc, char * argv[])
{
	int i;
	int pid[argc];

	setbuf(stdout, NULL);
	for (i=1; i<argc; i++)
		if ((pid[i] = fork()) == 0) { echo(argv[i]); exit(0); }
	for (i=1; i<argc; i++)
		waitpid(pid[i], NULL, 0);

	return 0;
}
