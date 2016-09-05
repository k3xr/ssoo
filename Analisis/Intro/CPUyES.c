/* CPUyES.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.1		Fecha: 21 Oct 2005
 * Descripción:
 *	Un total de 10000 veces se crean una serie de subdirectorios
 *	que luego se borran.
 *	La llamada sync fuerza que los cambios realizados vayan a disco.
 *	Como resultado tenemos un proceso que combina uso de CPU y ES.
 * Uso:
 *	CPUyES
 */
#define MYNAME	"CPUyES"

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i;

	for (i=0; i<10000; i++) {
		mkdir("tmpdir", 0777);
		mkdir("tmpdir/subdir", 0777);
		mkdir("tmpdir/subdir/subdir", 0777);
		sync();	/* Fuerza la escritura en disco */
		rmdir("tmpdir/subdir/subdir");
		rmdir("tmpdir/subdir");
		rmdir("tmpdir");
		sync();	/* Fuerza la escritura en disco */
	}

	return 0;
}
