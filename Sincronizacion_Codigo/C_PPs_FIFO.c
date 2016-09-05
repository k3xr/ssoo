/* C_PPs_FIFO.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Consumidor de un Fifo.
 *	Muestra por su salida estándard lo que lee del fichero indicado,
 *	convertido a mayúsculas.
 * Uso:
 *	C_PPs_FIFO fifoname
 */
#define MYNAME	"C_PPs_FIFO"

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	char ch;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror(MYNAME": open()");
		exit(1);
	}
	while(read(fd, &ch, 1) == 1) {
		ch = toupper(ch);
		write(1, &ch, 1);
	}
	return 0;
}
