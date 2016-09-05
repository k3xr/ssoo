/* CPUs_pth.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 01 May 2006
 * Descripción:
 *	Se lanzan los hilos indicados como primer argumento.
 *	Se esperan los segundos indicados como segundo argumento,
 *	Los hilos son de CPU intensiva y van incrementando una variable propia.
 *	Vencido el tiempo de espera se paran los hilos y
 *	se calcula y muestra la cantidad total de cómputo realizada.
 * Uso:
 * 	CPUs_pth [nthrds:1..10 [nscecs:1..60]]
 */
#define MYNAME	"CPUs_pth"

#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void CPUa100(long long * ptr)
{
	*ptr = 0;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	sched_yield();

	while(1)
		(*ptr)++;
}

int main(int argc, char * argv[])
{
	int i, n = 1, s = 10;
	pthread_t * tids;
	long long ** work, total = 0;

	switch(argc) {
	case 3:
		s = atoi(argv[2]);
		if (s >= 60) s = 60;
		if (s <= 01) s = 01;
	case 2:
		n = atoi(argv[1]);
		if (n >= 10) n = 10;
		if (n <= 01) n = 01;
	case 1:
		break;
	default:
		printf("Uso: "MYNAME" [nthrds:1..10 [nsecs:1..60]]\n");
		return 1;
	}

	tids = calloc(sizeof(pthread_t), n);
	work = calloc(sizeof(long long *), n);

	for (i = 0; i < n; i++) {
		work[i] = calloc(sizeof(long long), 1);
		pthread_create(&tids[i], NULL, (void*)CPUa100, (void*)work[i]);
	}

	usleep(s*1000*1000);

	for (i = 0; i < n; i++)
		pthread_cancel(tids[i]);
	for (i = 0; i < n; i++)
		pthread_join(tids[i], NULL);
	for (i = 0; i < n; i++)
		total += *(work[i]);
	printf("Total work done by %d threads in %d seconds is = %lld\n", n, s, total);
	fflush(stdout);
	return 0;
}
