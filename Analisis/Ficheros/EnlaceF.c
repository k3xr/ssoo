/* EnlaceF.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Pedro de Miguel
 * Versión: 1.0		Fecha: 21 enero 2006
 * Descripción:
 *	Realiza la llamada link con los argumentos suministrados.
 * Uso:
 *	EnlaceF fich_existente nuevo_nombre
 */
#define MYNAME	"EnlaceF"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	if (argc == 3) {
		link(argv[1], argv[2]);
	} else {
		printf("USO: %s fich_existente nuevo_nombre\n", MYNAME);
	}
	return 0;
}
