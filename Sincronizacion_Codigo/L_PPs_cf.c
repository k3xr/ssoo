/* L_PPs_cf.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Polit�cnica de Madrid, Facultad de Inform�tica
 *
 * Autor: Francisco Rosales
 * Versi�n: 1.0		Fecha: 9 May 2003
 * Descripci�n:
 *	Proceso Pesado Lector sobre un fichero compartido (BD).
 *	Sincronizaci�n v�a Cerrojos sobre el Fichero.
 *	Va mostrando el valor entero contenido en el ficehro.
 * Uso:
 *	L_PPs_cf
 */
#define MYNAME	"L_PPs_cf"

#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int fd, val = 0, cnt;
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
		/*Lecturas simult�neas del recurso compartido*/
			sched_yield();
			printf("%d\n", val);
		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &fl);
	}
	return 0;
}
