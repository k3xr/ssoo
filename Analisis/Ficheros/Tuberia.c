/* Tuberia.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Pedro de Miguel y  Francisco Rosales
 * Versión: 1.0		Fecha: 23 Sep 2006
 * Descripción:
 *	Crea una tubería y lanza un hijo, con el que se comunica
 *	por la tubería.
 * Uso:
 *	Tuberia
 */
#define MYNAME	"Tuberia"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_BUF		10

int main(void)
{
	int fd[2], nleidos;
	char buffer[MAX_BUF];

	if (pipe(fd) < 0) {
		perror(MYNAME": Error al crear el pipe\n");
		return 1;
	}

	switch(fork()) {
	case -1:
		perror(MYNAME": fork()");
		return 2;
	case  0:
		close (fd[1]);
		do {
			nleidos = read(fd[0], buffer, MAX_BUF);
			if (nleidos < 0) {
				perror(MYNAME" - PROCESO Y: Error en la lectura\n");
			} else {
				fprintf(stderr, MYNAME" - PROCESO Y: %d datos: ", nleidos);
				write (1, buffer, nleidos);
				fprintf(stderr, "\n");
			}
		} while (nleidos > 0);
		fprintf(stderr, MYNAME" - PROCESO Y: termina correctamente\n");
		break;
	default:
		close (fd[0]);
		do {
		nleidos = read(0, buffer, MAX_BUF);
			fprintf(stderr, MYNAME" - PROCESO X: %d datos a enviar\n", nleidos);
			write (fd[1], buffer, nleidos);
		} while (nleidos > 0);
		fprintf(stderr, MYNAME" - PROCESO X: termina correctamente\n");
		
	}
	return 0;
}
