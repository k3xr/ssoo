/* Crear_Proceso.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Modificado Por: Luis Miguel Mazaira
 * Versión: 12.0		Fecha: 20 Sep 2004
 * Descripción:
 *	Utiliza la llamada fork para crear un nuevo proceso.
 *	Se separan los caminos: padre, hijo y error con un switch.
 *
 * Uso:
 *	Crear_Proceso
 */
#define MYNAME	"Crear_Proceso"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int ret;
	
	ret = fork();
	
	switch(ret) 
	{
		case -1:
			perror(MYNAME": fork()");
			break;
		case  0:
			printf(MYNAME": \tPID=%d\tPPID=%d\n", getpid(), getppid());
			break;
		default:
			printf(MYNAME": \tPID=%d\tPPID=%d\n", getpid(), getppid());
	}

	pause();
	return 0;
}
