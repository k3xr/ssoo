/* Udir.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 23 Ene 2006
 * Descripción:
 *	Haciendo uso de las funciones de la familia "opendir",
 *	se lee el contenido del directorio especificado en el argumento.
 * Uso:
 *	Udir directorio
 */
#define MYNAME	"Udir"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	DIR * dir;
	struct dirent *entrada;

	if (argc != 2) {
		printf("Uso: "MYNAME" directorio\n");
		exit(0);
	}

	dir = opendir(argv[1]);
	if (dir == NULL) {
		perror(MYNAME": opendir()");
		exit(1);
	}
	
	printf("%10s   %s\n", "Nodo_i", "Nombre");
	while ((entrada = readdir(dir)) != NULL) {
		printf("%10ld   %s\n", entrada->d_ino, entrada->d_name);
	}
	if (!entrada && errno != 0) {
		perror(MYNAME": readdir()");
		exit(1);
	}

	closedir(dir);
	return 0;
}
