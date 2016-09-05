/* Enmascarado.c*/
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 11 Nov 2005
 * Descripción:
 * 	Realiza la llamada CREAT sobre el fichero y con los permisos
 * 	indicados, previo cambio de la máscara de creación de ficheros
 * 	al valor indicado.
 * 	Los valores por defecto son: permisos 0666 y máscara 0000.
 * Uso:
 *	Enmascarado fichero [permisos [mascara]]
 */
#define MYNAME "Enmascarado"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	char * archivo = NULL;
	int permisos = 0666, mascara = 0000;
	int ret, ret1 = -1, ret2 = -1;
	struct stat sts[1];
	int fd;

	if (argc > 1)
		archivo = argv[1];
	if (argc > 2)
		ret1 = sscanf(argv[2], "%o", &permisos);
	if (argc > 3)
		ret2 = sscanf(argv[3], "%o", &mascara);
	if (!archivo || !ret1 || !ret2 || argc > 4) {
		printf("Uso: "MYNAME" fichero [permisos [mascara]]\n");
		exit(0);
	}

	if (ret2 == 1) {
		printf(MYNAME" Cambiando máscara a 0%03o\n", mascara);
		mascara = umask(mascara);
		if (mascara < 0) {
			perror(MYNAME": umask()");
			exit(1);
		}
		printf(MYNAME" La máscara anterior era 0%03o\n", mascara);
	} else {
		mascara = umask(0);
		umask(mascara);
		printf(MYNAME" La máscara actual es 0%03o\n", mascara);
	}

	ret = stat(archivo, sts);
	if (ret < 0) {
		printf(MYNAME" Creando archivo \"%s\"\n", archivo);
		printf(MYNAME" Aplicando permisos 0%03o\n", permisos);
	} else {
		printf(MYNAME" Truncando archivo existente \"%s\"\n", archivo);
		printf(MYNAME" Permisos actuales del archivo 0%03o\n", sts->st_mode & ~S_IFMT);
	} 
	fd = creat(archivo, permisos);
	if (fd < 0) {
		perror(MYNAME": creat()");
		exit(1);
	}

	ret = fstat(fd, sts);
	if (ret < 0) {
		perror(MYNAME": fstat()");
		exit(1);
	}

	printf(MYNAME" Tamaño final del archivo %d\n", (int) sts->st_size);
	printf(MYNAME" Permisos finales del archivo 0%03o\n", sts->st_mode & ~S_IFMT);

	return 0;
}
