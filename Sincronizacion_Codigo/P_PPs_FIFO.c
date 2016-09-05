/* P_PPs_FIFO.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Productor sobre un Fifo.
 *	Escribe sobre el fifo lo que lee de su entrada estándard.
 * Uso:
 *	P_PPs_FIFO fifoname
 */
#define MYNAME	"P_PPs_FIFO"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd;
	char ch;
	fd = creat(argv[1], 0666);
	if (fd == -1) {
		perror(MYNAME": creat()");
		return 1;
	}
	while(read(0, &ch, 1) == 1)
		write(fd, &ch, 1);
	return 0;
}
