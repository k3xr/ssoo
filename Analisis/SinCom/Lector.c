/* L_PPs_cf.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Proceso Pesado Lector sobre un fichero compartido (BD).
 *	Va mostrando el valor entero contenido en el ficehro.
 * Uso:
 *	Lector
 */
#define MYNAME	"Lector"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fd, val=0, cnt;
	struct flock fl;

	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = getpid();
	fd = open("BD", O_RDONLY);
	for (cnt = 0; cnt < 10; cnt++)
	{
		sched_yield();
		fl.l_type = F_RDLCK;
		fcntl(fd, F_SETLKW, &fl);
		lseek(fd, 0, SEEK_SET);
		read(fd, &val, sizeof(int));
		sched_yield();
		printf("%d\n", val);
		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &fl);
	}
	return 0;
}
