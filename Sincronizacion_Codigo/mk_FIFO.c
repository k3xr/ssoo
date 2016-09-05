/* mk_FIFO.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Creador de un Fifo.
 * Uso:
 *	mk_FIFO fifoname
 */
#define MYNAME	"mk_FIFO"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (mkfifo(argv[1], 0666) == 0)
		return 0;
	perror(MYNAME": mkfifo()");
	return 1;
}
