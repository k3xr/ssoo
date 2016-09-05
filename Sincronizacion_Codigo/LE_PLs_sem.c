/* LE_PLs_sem.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Procesos Ligeros implementando Lectores y Escritores sobre un RECURSO_COMPARTIDO.
 *	Sincronización vía semáforos.
 * Uso:
 *	LE_PLs_sem
 */
#define MYNAME	"LE_PLs_sem"

#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

void CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU(void)
{
	static int veces = 0;
	veces += getpid() + 1;
	veces ^= times(0);
	veces %= 8 * 8;
	usleep(veces*10);
}

volatile int RECURSO_COMPARTIDO = 0;

sem_t en_exclusiva;			/* Acceso a dato */
sem_t de_lectura;			/* Acceso a n_lectores */
int n_lectores = 0;			/* Número de lectores */

void Lector(void)
{
	int i;

	sem_wait(&de_lectura);
	n_lectores = n_lectores + 1;
	if (n_lectores == 1)
		sem_wait(&en_exclusiva);
	sem_post(&de_lectura);

	/*Lecturas simultáneas del recurso compartido*/
	for (i = 0; i < 100; i++) {
		printf("\r      	%d R  \r", RECURSO_COMPARTIDO);
		fflush(stdout);
		CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU();
	}

	sem_wait(&de_lectura);
	n_lectores = n_lectores - 1;
	if (n_lectores == 0)
		sem_post(&en_exclusiva);
	sem_post(&de_lectura);

	pthread_exit(0);
}

void Escritor(void)
{
	int i;

	sem_wait(&en_exclusiva);
	/*Acceso en exclusiva al recurso compartido*/
		printf("\r	%d    \r", RECURSO_COMPARTIDO);
		fflush(stdout);
	for (i = 0; i < 100; i++) {
		printf("\r %d	\r", i);
		fflush(stdout);
		RECURSO_COMPARTIDO++;
		CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU();
	}
		printf("\r	%d    \r", RECURSO_COMPARTIDO);
		fflush(stdout);
	sem_post(&en_exclusiva);

	pthread_exit(0);
}

#define NRDS	300
#define NWRS	10

int main(void)
{
	pthread_t rds[NRDS];
	pthread_t wrs[NWRS];
	int i;

	sem_init(&en_exclusiva, 0, 1);	/* Situación inicial */
	sem_init(&de_lectura, 0, 1);

	for (i = 0; (i < NRDS) || (i < NWRS); i++) {	/* Arranque */
		if (i < NWRS)
			pthread_create(&(wrs[i]), NULL, (void*)Escritor, NULL);
		if (i < NRDS)
			pthread_create(&(rds[i]), NULL, (void*)Lector, NULL);
	}

		CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU();

	for (i = 0; (i < NRDS) || (i < NWRS); i++) {	/* Esperar terminación */
		if (i < NRDS)
			pthread_join(rds[i], NULL);	
		if (i < NWRS)
			pthread_join(wrs[i], NULL);	
	}

	sem_destroy(&de_lectura);	/* Destruir */
	sem_destroy(&en_exclusiva);

	printf("\n");
	fflush(stdout);
	pthread_exit(0);
	return 0;
}
