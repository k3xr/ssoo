/* P_PPs_sem.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Productor sobre un fichero proyectado compartido (BUFFER).
 *	Sincronización vía semáforos con nombre.
 *	Escribe en el buffer valores numéricos crecientes.
 * Uso:
 *	P_PPs_sem
 */
#define MYNAME	"P_PPs_sem"

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

void Productor(void)
{
	int i, dato;

	for (i = 0; i < TOTAL_DATOS; i++) {
		/*Producir el dato*/
		dato = i;

		sem_wait(n_huecos);	/* Un hueco menos */
		buffer[i % BUFF_SIZE] = dato;	/* METER */
		sem_post(n_datos);	/* Un dato más */
	}
}


int main(void)
{
	int shd = open("BUFFER", O_CREAT | O_WRONLY, 0600);

	ftruncate(shd, SIZE);
	buffer = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shd, 0);
	n_datos = sem_open("N_DATOS", O_CREAT, 0700, 0);
	n_huecos = sem_open("N_HUECOS", O_CREAT, 0700, BUFF_SIZE);
	Productor();
	munmap(buffer, SIZE);
	close(shd);
	unlink("BUFFER");
	sem_close(n_datos);
	sem_unlink("N_DATOS");
	sem_close(n_huecos);
	sem_unlink("N_HUECOS");

	return 0;
}
