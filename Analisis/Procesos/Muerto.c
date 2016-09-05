/* Muerto.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 20 Sep 2004
 * MOdificado por: Luis Miguel Mazaira
 * Versión: 1.1		Fecha: 9 Sep 2009
 * Descripción:
 *	Utiliza la llamada fork para crear un nuevo proceso.
 * Uso:
 *	Muerto
 */
#define MYNAME	"Muerto"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int ret;

	ret = fork();
	switch(ret) {
	case -1:
		perror(MYNAME": fork()");
		break;
	case  0:
		printf(MYNAME": \tPID=%d\tPPID=%d\n", getpid(), getppid());
		exit(0);
		break;
	default:
		printf(MYNAME": \tPID=%d\tPPID=%d\n", getpid(), getppid());
	}

	pause();		
	return 0;
}
