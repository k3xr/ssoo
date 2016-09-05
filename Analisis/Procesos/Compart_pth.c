/* Compart_pth.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 21 Sep 2004
 * Descripción:
 *	El programa demuestra como los hilos de un proceso,
 *	a diferencia de lo que sucede con padre e hijo, comparten memoria.
 * Compilación: gcc -Wall  -lpthread  Compart_pth.c -o Compart_pth
 * Uso:
 *	Compart_pth
 */
#define MYNAME	"Compart_pth"

#include <pthread.h>
#include <stdio.h>


unsigned acumulador = 0;

void * echo(void * dummy)
{
	setbuf(stdout, NULL);
	while(1) {
		printf(MYNAME": Acumulador = %u \r", acumulador);
		sched_yield();
	}
	return NULL;
}

int main(void)
{
	pthread_t tid;


	pthread_create(&tid, NULL, echo, NULL);

	while(1) {
		acumulador++;
		sched_yield();
	}
	return 0;
}
