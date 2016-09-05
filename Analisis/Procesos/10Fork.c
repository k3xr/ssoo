/* 10Fork.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Modificado por: Luis Miguel Mazaira
 * Versión: 12.		Fecha: 17 Sept 2009
 * Descripción:
 * Uso:
 *	10Fork
 */
#define MYNAME	"10Fork"

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	pid_t pid;
	int i;
	int n = 10;

	for (i = 0; i < n; i++) {
		pid = fork();
		if (pid == 0)	
			break;
	}
	printf(MYNAME": [%d] El padre del proceso %d es %d\n", i, getpid(), getppid());
	sleep(1);
	return 0;
}
