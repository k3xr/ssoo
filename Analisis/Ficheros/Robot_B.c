/* Robot_B.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 11 Nov 2005
 * Descripción:
 * 	Este programa actua como un robot dirigido por los caracteres
 * 	que lee de la entrada estandar.
 * 	Los dígitos se agregan al valor de "offset".
 * 	'-' o '+' hace offset del signo indicado.
 * 	'<' '=' '>' realiza un lseek relativo a: principio, posición
 * 	actual o final de la salida estándar, respectivamente.
 * 	Letras minúsculas o mayúsculas se escriben en la salida estandar.
 * 	El @ termina el proceso.
 * 	Cualquier otro caracter se ignora.
 * Uso:
 *	Robot fichero [modo]
 */
#define MYNAME "Robot_B"

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	char ch;
	off_t offset =  0;
	int   whence = -1;
	char * ptr = 0;
	int perm = 0777;
	int mode = 0;
	char * file = 0;
	int fd;

	switch (argc) {
	case 3:	/* mode */
		mode = strtol(argv[2], &ptr, 8);
		if (!ptr || *ptr) break;
	case 2:	/* file */
		file = argv[1];
	}

	fd = open(file, mode, perm);
	if (fd < 0) {
		perror(MYNAME": open");
		return 1;
	}
	
	while (read(0, &ch, 1) > 0) {
		switch (ch) {
		case '0' ... '9':
			offset *= 10;
			offset += ch - '0';
			break;
		case '-': if (offset > 0) offset = -offset;
			break;
		case '+': if (offset < 0) offset = -offset;
			break;
		case '<': if (whence < 0) whence = SEEK_SET;
		case '=': if (whence < 0) whence = SEEK_CUR;
		case '>': if (whence < 0) whence = SEEK_END;
			offset = lseek(fd, offset, whence);
			if (offset < 0)
				perror(MYNAME": lseek()");
			offset =  0;
			whence = -1;
			break;
		case 'a' ... 'z':
		case 'A' ... 'Z':
			if (write(fd, &ch, 1) != 1)
				perror(MYNAME" write()");
			break;
		case '@':
			close(fd);
			read(0, &ch, 1);
			return 0;
		default:
			offset =  0;
			break;
		}
	}
	return 0;
}
