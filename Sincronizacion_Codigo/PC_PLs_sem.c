/* PC_PLs_sem.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Polit�cnica de Madrid, Facultad de Inform�tica
 *
 * Autor: Francisco Rosales
 * Versi�n: 1.0		Fecha: 9 May 2003
 * Descripci�n:
 *	Procesos Ligeros implementando Productor y Consumidor comunicados por memoria compartida.
 *	Sincronizaci�n v�a sem�foros.
 * Uso:
 *	PC_PLs_sem
 */
#define MYNAME	"PC_PLs_sem"

#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define BUFF_SIZE      1024
#define TOTAL_DATOS  100000

sem_t n_datos;				/* datos en buffer compartido */
sem_t n_huecos;				/* huecos en buffer compartido */

int buffer[BUFF_SIZE];			/* buffer acotado compartido */

void Productor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		/*Producir el dato*/
		dato = i;

		sem_wait(&n_huecos);	/* Un hueco menos */
		buffer[i % BUFF_SIZE] = dato;	/* METER */
		sem_post(&n_datos);	/* Un dato m�s */
	}
}

void Consumidor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		sem_wait(&n_datos);	/* Un dato menos */
		dato = buffer[i % BUFF_SIZE];	/* SACAR */
		sem_post(&n_huecos);	/* Un hueco m�s */

		/*Consumir el dato*/
		printf("\r %d   \r", dato);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}

int main(void)
{
	pthread_t th1, th2;

	sem_init(&n_datos, 0, 0);	/* Situaci�n inicial */
	sem_init(&n_huecos, 0, BUFF_SIZE);
	pthread_create(&th1, NULL, (void*)Productor, NULL);	/*Arranque */
	pthread_create(&th2, NULL, (void*)Consumidor, NULL);
	pthread_join(th1, NULL);	/* Esperar terminaci�n */
	pthread_join(th2, NULL);
	sem_destroy(&n_datos);		/* Destruir */
	sem_destroy(&n_huecos);
	return 0;
}
