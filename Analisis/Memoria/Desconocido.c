/* Desconocido.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *      Desconocido: ¿Qué hace este programa?
 *
 * Uso:
 *      Desconocido fichero_origen fichero_destino
 *
 */
#define MYNAME	"Desconocido"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd;
	int tam;
	char *org, *p, *q;
	struct stat bstat;

	if (argc!=3) {
		fprintf(stderr, "Uso: %s archivo_origen archivo_destino\n", MYNAME);
		return(1);
	}

	if ((fd=open(argv[1], O_RDONLY))<0) {
		perror(MYNAME": No puede abrirse el archivo");
		return(1);
	}

	if (fstat(fd, &bstat)<0) {
		perror(MYNAME": Error en fstat del archivo");
		close(fd);
		return(1);
	}

	if ((org=mmap((caddr_t) 0, bstat.st_size, PROT_READ|PROT_WRITE,	MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror(MYNAME": Error en la proyeccion del archivo");
		close(fd);
		return(1);
	}

	close(fd);

	p=org;

        for ( ; p<org+bstat.st_size; p++)
                (*p)=toupper(*p);

        if ((fd=open(argv[2], O_CREAT|O_TRUNC|O_RDWR, 0640))<0) {
        	perror(MYNAME": No puede crearse el archivo destino");
        	exit(1);
        }
        
	if (ftruncate(fd, bstat.st_size)<0) {
        	perror(MYNAME": Error en ftruncate del archivo destino");
        	close(fd);
        	unlink (argv[2]);
        	exit(1);
        }
        
	if ((q=mmap((caddr_t) 0, bstat.st_size, PROT_WRITE,MAP_SHARED, fd, 0)) == MAP_FAILED) {
        	perror(MYNAME": Error en la proyeccion del archivo destino");
        	close(fd);
        	unlink (argv[2]);
        	exit(1);
        }

        memcpy(q, org, bstat.st_size);

	munmap(p, bstat.st_size);
        munmap(q, bstat.st_size);

	return(0);
}
