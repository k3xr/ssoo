/* Concurr_pth.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 21 Sep 2004
 * Descripción:
 *	El programa demuestra la concurrencia entre hilos de un proceso.
 *	Se crean tantos hilos como argumentos de invocación, y cada
 *	hilo consume cierta CPU e imprime el correspondiente argumento.
 * Compilación: gcc -Wall  -lpthread  Concurr_pth.c -o Concurr_pth
 * Uso:
 *	Concurr_pth A B C D E F ...
 */
#define MYNAME	"Concurr_pth"

#include <pthread.h>
#include <stdio.h>



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
	pthread_t tid[argc];

	setbuf(stdout, NULL);
	for (i=1; i<argc; i++)
		pthread_create(&tid[i], NULL, echo, argv[i]);
	for (i=1; i<argc; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
