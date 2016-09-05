/* Redirige.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Pedro de Miguel
 * Versión: 1.0		Fecha: 21 enero 2006
 * Descripción:
 *	Redirige la entrada y salida estándar y ejecuta el programa
 *	especificado como tercer argumento.
 * Uso:
 *	Redirige fichero_entrada fichero_salida programa [argumentos]
 */
#define MYNAME	"Redirige"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	int fd1, fd2;
	if (argc > 3) {
		fd1 = open(argv[1], O_RDONLY);
		if (fd1 < 0)   {
			perror(MYNAME": 1"); return 1;
		}
		fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0600);
		if (fd2 < 0)   {
			close(fd1);
			perror(MYNAME": 2"); return 2;
		}
		close(0);
		dup(fd1);	
		close(fd1);
		close(1);
		dup(fd2);
		close(fd2);
		execvp(argv[3], &argv[3]);
		perror(MYNAME": 3"); return 3;
		
	} else {
		printf("USO: %s fichero_entrada fichero_salida programa [argumentos]\n", MYNAME);
	}
	return 0;
}
