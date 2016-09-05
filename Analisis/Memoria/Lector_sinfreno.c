/* Lector_sinfreno.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira
 * Descripción:
 *      Lector_sinfreno: Lee un fichero proyectado saliéndose de la zona
 *	proyectada para ilustrar qué ocurre en esta situación.
 *
 * Uso:
 *      Lector_sinfreno fichero
 *
 */
#define MYNAME	"Lector_sinfreno"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

char *org;

void mostrar_mapa(int sen)
{
        char mandato[256];
        int mipid;

	
        printf("ERROR ACCEDIENDO A DIRECCIÓN :%p \n",org);
        printf("-------------------------------------------------------------------\n");
        mipid= getpid();
        sprintf(mandato, "pmap %d", mipid);
        system(mandato);
        printf("-------------------------------------------------------------------\n");
	exit(0);
}

int main(int  argc, char **argv) {
	int fd, i;
	struct stat bstat;

	signal(SIGSEGV, mostrar_mapa);

	if (argc!=2) {
		fprintf (stderr, "Uso: %s archivo\n", MYNAME);
		return(1);
	}

	/* Abre el archivo para lectura */
	if ((fd=open(argv[1], O_RDONLY))<0) {
		perror(MYNAME": No puede abrirse el archivo");
		return(1);
	}

	/* Averigua el numero de caracteres del archivo */
	if (fstat(fd, &bstat)<0) {
		perror(MYNAME": Error en fstat del archivo");
		close(fd);
		return(1);
	}

	/* Asegura que no quede pegado a otra región */
	if (mmap((caddr_t) 0, 1, PROT_NONE,
				MAP_ANON|MAP_PRIVATE, -1, 0) == MAP_FAILED) {
		perror(MYNAME": Error en la proyeccion");
		close(fd);
		return(1);
	}

	/* Se proyecta el archivo */
	if ((org=mmap((caddr_t) 0, bstat.st_size, PROT_READ,
					MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror(MYNAME": Error en la proyeccion del archivo");
		close(fd);
		return(1);
	}

	/* Asegura que no quede pegado a otra región */
	if (mmap((caddr_t) 0, 1, PROT_NONE,
				MAP_ANON|MAP_PRIVATE, -1, 0) == MAP_FAILED) {
		perror(MYNAME": Error en la proyeccion");
		close(fd);
		return(1);
	}

	/* Se cierra el archivo */
	close(fd);

	for (i=0; ;i++)
	{
		if (i<bstat.st_size)
			printf("byte[%d](lectura dentro del fichero): %hhu\n", i, *org);
		else
			printf("byte[%d](lectura fuera del fichero): %hhu\n", i, *org);
		org++;
	}
	return(0);
}
