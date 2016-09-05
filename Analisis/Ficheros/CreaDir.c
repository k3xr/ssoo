/* CreaDir.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Pedro de Miguel
 * Versión: 1.0		Fecha: 21 enero 2006
 * Descripción:
 *	Realiza la llamada mkdir con los permisos suministrados.
 * Uso:
 *	CreaDir nombre [permisos]
 */
#define MYNAME	"CreaDir"

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	char * ptr = NULL;
	int perm = 0777;

	if (argc == 3) {
		perm = strtol(argv[2], &ptr, 8);
		if (!ptr || *ptr) return 1;
		mkdir(argv[1], perm);
	} else if (argc == 2){
		mkdir(argv[1], 0777);
	} else {
		printf("USO: %s nombre [permisos]\n", MYNAME);
	}
	return 0;
}
