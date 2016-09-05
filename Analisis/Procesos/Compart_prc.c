/* Compart_prc.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 21 Sep 2004
 * Descripción:
 *	El programa demuestra como los procesos padre e hijo,
 *	a diferencia de lo que sucede con hilos, no comparten memoria.
 *
 * Uso:
 *	Compart_prc
 */
#define MYNAME	"Compart_prc"

#include <sched.h>
#include <stdio.h>
#include <unistd.h>

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

	switch(fork()) 
	{
		case -1: return 1;
		case  0: echo(NULL);
		default:
			while(1) 
			{
			acumulador++;
			sched_yield();
			}
	}
	return 0;
}
