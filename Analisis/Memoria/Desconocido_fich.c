/* Misterio_fich.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *      Misterio_fich: ¿Qué error hay en este programa que hace que no
 *	se comporte como el programa "Misterio"?
 *
 * Uso:
 *      Misterio_fich fichero
 *
 */
#define MYNAME	"Misterio_fich"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int  argc, char **argv) {
	char aux;
	int fd_1;
	int fd_2;

	if (argc!=3) {
		fprintf (stderr, "Uso: %s archivo_origen archivo_destino\n", MYNAME);
		return(1);
	}

	if ((fd_1=open(argv[1], O_RDWR))<0) {
		perror(MYNAME": No puede abrirse el archivo");
		return(1);
	}

	if ((fd_2=open(argv[2], O_CREAT|O_TRUNC|O_RDWR, 0640))<0) {
                 perror(MYNAME": No puede crearse el archivo destino");
                 exit(1);
         }

	while (read(fd_1, &aux, 1)) {
		aux=toupper(aux);
		write(fd_2, &aux, 1);
	}


	close(fd_1);
	close(fd_2);
	return(0);
}
