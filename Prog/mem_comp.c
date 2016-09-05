/* mem_comp.c */
/*
 * Copyright 2010 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 18 May 2010
 * Descripción:
 *	Ejemplo que ilustra la creación de regiones de memoria compartida
 *	utilizando servicios Posix 
 * Uso:
 *	mem_comp 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
	/* Identificador asociado al segmento de memoria compartida */
	int segment_id;
	/* Puntero al segmento de memoria compartida */
	char* shared_memory;
	/* Tamaño del segmento de memoria compartida */ 
	const int segment_size = 4096; /* bytes */

	/* Reservar espacio en memoria */
	segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);
	if (segment_id < 0){
		perror("shmget");
		exit(1);
	}

	/* Asocia el segmento de memoria compartida al espacio de memoria
	del proceso */
	shared_memory = (char *) shmat(segment_id, NULL, 0);
	if (shared_memory < 0){
		perror("shmat");
		exit(1);
	}
	printf("Región de memoria compartida %d que comienza en la dirección %p\n", segment_id, shared_memory);

	/* Escribimos en la región de memoria compartida */
	sprintf(shared_memory, "¡Hola chicos!");

	/* Leemos de la región de memoria compartida escribiendo por pantalla */
	printf("*%s*\n", shared_memory);

	/* Liberamos la región de memoria compartida de la imagen de memoria
	del proceso */ 
	if ( shmdt(shared_memory) == -1) {
		perror("shmdt");
		exit(1);
	}

	/** Eliminamos la región de memoria compartida */
	if ( shmctl(segment_id, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}

	return 0;
}
