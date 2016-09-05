/* C_PPs_sem.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Consumidor de un fichero proyectado compartido (BUFFER).
 *	Sincronización vía semáforos con nombre.
 *	Muestra por su salida estándard el valor numérico entero que lee del buffer.
 * Uso:
 *	C_PPs_sem
 */
#define MYNAME	"C_PPs_sem"

#include <sys/mman.h>

#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE	1024
#define SIZE		(BUFF_SIZE * sizeof(int))
#define TOTAL_DATOS	100000

sem_t *n_datos, *n_huecos;		/* Punteros a semáforo */
int *buffer;				/* Puntero a memoria compartida */

void Consumidor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		sem_wait(n_datos);	/* Un dato menos */
		dato = buffer[i % BUFF_SIZE];	/* SACAR */
		sem_post(n_huecos);	/* Un hueco más */

		/*Consumir el dato*/
		printf("\r %d   \r", dato);
		fflush(stdout);
	}
	printf("\n");
	fflush(stdout);
}

int main(void)
{
	int shd;

	while ((shd = open("BUFFER", O_RDONLY)) == -1)
		continue;
	buffer = mmap(0, SIZE, PROT_READ, MAP_SHARED, shd, 0);
	n_datos = sem_open("N_DATOS", 0);
	n_huecos = sem_open("N_HUECOS", 0);
	Consumidor();
	munmap(buffer, SIZE);
	close(shd);
	sem_close(n_datos);
	sem_close(n_huecos);

	return 0;
}
