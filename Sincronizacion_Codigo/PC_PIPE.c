/* PC_PIPE.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Productor y Consumidor comunicados por un pipe.
 *	El Productor emite por el pipe lo que lee de la entrada estandard.
 *	El Consumidor emite por la salida estandard lo que lee del pipe.
 * Uso:
 *	PC_PIPE
 */
#define MYNAME	"PC_PIPE"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fds[2];
	int ret;
	char byte;

	ret = pipe(fds);
	if (ret == -1) {
		perror(MYNAME": pipe");
		exit(1);
	}
	switch (fork()) {
	case -1:
		perror(MYNAME": fork");
		exit(1);
	default:		/*Productor */
		while (read(0, &byte, 1) > 0)
			write(fds[1], &byte, 1);
		break;
	case 0:			/*Consumidor */
		while (read(fds[0], &byte, 1) > 0)
			write(1, &byte, 1);
		break;
	}
	return 0;
}
