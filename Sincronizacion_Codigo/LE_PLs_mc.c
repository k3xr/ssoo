/* LE_PLs_mc.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Procesos Ligeros implementando Lectores y Escritores sobre un RECURSO_COMPARTIDO.
 *	Sincronización vía mutex y variables condicionales.
 * Uso:
 *	LE_PLs_mc
 */
#define MYNAME	"LE_PLs_mc"

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

pthread_mutex_t mutex;			/* Control de acceso */
pthread_cond_t a_leer, a_escribir;	/* Condiciones de espera */
int leyendo, escribiendo;		/* Estado del acceso */

void Lector(void)
{
	int i;

	pthread_mutex_lock(&mutex);
	while (escribiendo)
		pthread_cond_wait(&a_leer, &mutex);
	leyendo++;
	pthread_mutex_unlock(&mutex);

	/*Lecturas simultáneas del recurso compartido*/
	for (i = 0; i < 100; i++) {
		printf("	%d  \n", RECURSO_COMPARTIDO);
		fflush(stdout);
		CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU();
	}

	pthread_mutex_lock(&mutex);
	leyendo--;
	if (!leyendo)
		pthread_cond_signal(&a_escribir);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void Escritor(void)
{
	int i;

	pthread_mutex_lock(&mutex);
	while (leyendo || escribiendo)
		pthread_cond_wait(&a_escribir, &mutex);
	escribiendo++;
	pthread_mutex_unlock(&mutex);

	/*Acceso en exclusiva al recurso compartido*/
	for (i = 0; i < 100; i++) {
		printf("\r %d 		", i);
		fflush(stdout);
		RECURSO_COMPARTIDO++;
		CONSUMO_ALEATORIO_DE_TIEMPO_DE_CPU();
	}

	pthread_mutex_lock(&mutex);
	escribiendo--;
	pthread_cond_signal(&a_escribir);
	pthread_cond_broadcast(&a_leer);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

int main_(void)
{
	pthread_t th1, th2, th3, th4;

	pthread_mutex_init(&mutex, NULL);	/* Situación inicial */
	pthread_cond_init(&a_leer, NULL);
	pthread_cond_init(&a_escribir, NULL);
	pthread_create(&th1, NULL, (void*)Lector, NULL);	/* Arranque */
	pthread_create(&th2, NULL, (void*)Escritor, NULL);
	pthread_create(&th3, NULL, (void*)Lector, NULL);
	pthread_create(&th4, NULL, (void*)Escritor, NULL);
	pthread_join(th1, NULL);	/* Esperar terminación */
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	pthread_mutex_destroy(&mutex);	/* Destruir */
	pthread_cond_destroy(&a_leer);
	pthread_cond_destroy(&a_escribir);

	return 0;
}

#define NRDS	300
#define NWRS	10

int main(void)
{
	pthread_t rds[NRDS];
	pthread_t wrs[NWRS];
	int i;

	pthread_mutex_init(&mutex, NULL);	/* Situación inicial */
	pthread_cond_init(&a_leer, NULL);
	pthread_cond_init(&a_escribir, NULL);

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

	pthread_mutex_destroy(&mutex);	/* Destruir */
	pthread_cond_destroy(&a_leer);
	pthread_cond_destroy(&a_escribir);

	printf("\n");
	pthread_exit(0);
	return 0;
}
