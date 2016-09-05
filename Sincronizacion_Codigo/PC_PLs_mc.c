/* PC_PLs_mc.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Procesos Ligeros implementando Productor y Consumidor comunicados por memoria compartida.
 *	Sincronización vía mutexes y variables condicionales.
 * Uso:
 *	PC_PLs_mc
 */
#define MYNAME	"PC_PLs_mc"

#include <sys/mman.h>

#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE      1024
#define TOTAL_DATOS  100000

int n_datos;				/* Datos en el buffer */
int buffer[BUFF_SIZE];			/* buffer circular compartido */
pthread_mutex_t mutex;			/* Acceso a sección crítica */
pthread_cond_t no_lleno, no_vacio;	/* Condiciones de espera */

void Productor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		/*Producir el dato*/
		dato = i;

		pthread_mutex_lock(&mutex);
		while (n_datos == BUFF_SIZE)
			pthread_cond_wait(&no_lleno, &mutex);
		buffer[i % BUFF_SIZE] = dato;
		n_datos++;
		pthread_cond_signal(&no_vacio);
		pthread_mutex_unlock(&mutex);
	}
}

void Consumidor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		pthread_mutex_lock(&mutex);
		while (n_datos == 0)
			pthread_cond_wait(&no_vacio, &mutex);
		dato = buffer[i % BUFF_SIZE];
		n_datos--;
		pthread_cond_signal(&no_lleno);
		pthread_mutex_unlock(&mutex);

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

	pthread_mutex_init(&mutex, NULL);	/* Situación inicial */
	pthread_cond_init(&no_lleno, NULL);
	pthread_cond_init(&no_vacio, NULL);
	pthread_create(&th1, NULL, (void*)Productor, NULL);	/* Arranque */
	pthread_create(&th2, NULL, (void*)Consumidor, NULL);
	pthread_join(th1, NULL);	/* Esperar terminación */
	pthread_join(th2, NULL);
	pthread_mutex_destroy(&mutex);	/* Destruir */
	pthread_cond_destroy(&no_lleno);
	pthread_cond_destroy(&no_vacio);

	return 0;
}
