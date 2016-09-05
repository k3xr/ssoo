/* Identificadores.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 20 Sep 2004
 * Descripción:
 *	Muestra por la salida estándar varios de los identificadores
 *	del proceso y de su propietario.
 * Uso:
 *	Identificadores
 */
#define MYNAME	"Identificadores"

#include <stdio.h>
#include <unistd.h>

int main(void)
{
	printf(MYNAME":        PID=%d\n",  getpid());
	printf(MYNAME":       PPID=%d\n", getppid());
	printf(MYNAME":        UID=%d\n",  getuid());
	printf(MYNAME":       EUID=%d\n", geteuid());
	printf(MYNAME":        GID=%d\n",  getgid());
	printf(MYNAME":       EGID=%d\n", getegid());
	return 0;
}
