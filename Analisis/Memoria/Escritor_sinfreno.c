/* Escritor_sinfreno.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira
 * Versión: 1.1		Fecha: 9 Sep 2009
 * Descripción:
 *      Escritor_sinfreno: Escribe en un fichero existente proyectado
 *	saliéndose de la zona proyectada para ilustrar qué ocurre.
 *
 * Uso:
 *      Escritor_sinfreno fichero
 *
 */
#define MYNAME	"Escritor_sinfreno"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

#define VALOR 'X'
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

	signal(SIGSEGV,mostrar_mapa);

	if (argc!=2) {
		fprintf(stderr, "Uso: %s archivo\n", MYNAME);
		return(1);
	}


        if ((fd=open(argv[1], O_RDWR))<0) {
                perror(MYNAME": No puede abrirse el archivo");
                return(1);
        }

        /* Averigua el número de caracteres del archivo */
        if (fstat(fd, &bstat)<0) {
                perror(MYNAME": Error en fstat del archivo");
                close(fd);
                return(1);
        }

        if (bstat.st_size==0)
                return(0);

	/* Asegura que no quede pegado a otra región */
	if (mmap((caddr_t) 0, 1, PROT_NONE,
				MAP_ANON|MAP_PRIVATE, -1, 0) == MAP_FAILED) {
		perror(MYNAME": Error en la proyeccion");
		close(fd);
		return(1);
	}

        /* Se proyecta el archivo */
        if ((org=mmap((caddr_t) 0, bstat.st_size, PROT_READ|PROT_WRITE,
                                        MAP_SHARED, fd, 0)) == MAP_FAILED) {
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

	for (i=0; ;i++) {
		(*org++)=VALOR;
		if (i<bstat.st_size)
			printf("escribe %hhu en byte[%d](escribe dentro del fichero)\n", VALOR, i);
		else
			printf("escribe %hhu en byte[%d](escribe fuera del fichero)\n", VALOR, i);
	}

	return(0);
}
