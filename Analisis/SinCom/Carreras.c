/* Carrera_B.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 23 Mar 2006
 * Descripción:
 *	Demuestra una condición de carrera en el acceso a una variable compartida.
 * Uso:
 *	Carrera_B
 */
#define MYNAME	"Carrera_B"

#include <sys/mman.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sched.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void shared_free(void * ptr, unsigned size)
{
	munmap(ptr, size);
}

void * shared_alloc(unsigned size)
{
	void * ptr = NULL;
	int fd = open("/dev/zero", O_RDWR);

	if (fd < 0) return NULL;
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	return ptr;
}


#define X(a)		{ (a); (a); (a); (a); (a); }

int main(void)
{
	int i, v;
	volatile int * Variable = shared_alloc(sizeof(int));

	for (i = 0; i < 50; i++) 
	{
		Variable[0] = 0;
		switch(fork()) 
		{
			case -1:
				perror(MYNAME": fork()");
				exit(1);
			case 0:
				for (v=0;v<10000;v++) X(X(Variable[0]++)); exit(0);
			default:
				for (v=0;v<10000;v++) X(X(Variable[0]--)); wait(NULL);
		}
		printf("\t%10d\n ", Variable[0]);
	}

	shared_free((void*)Variable, sizeof(int));
	return 0;
}

