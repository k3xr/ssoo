/* C_PPs_FIFO.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Polit�cnica de Madrid, Facultad de Inform�tica
 *
 * Autor: Francisco Rosales
 * Versi�n: 1.0		Fecha: 9 May 2003
 * Descripci�n:
 *	Proceso Pesado Consumidor de un Fifo.
 *	Muestra por su salida est�ndard lo que lee del fichero indicado,
 *	convertido a may�sculas.
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
