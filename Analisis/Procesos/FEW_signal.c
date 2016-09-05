/* FEW_signal.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 26 Oct 2005
 * Descripción:
 *	Muestra como las llamadas Fork y Exec afectan a la manipulación de señales.
 *	FEW = Fork+Exec+Wait, pero tras el Fork y antes del Exec, el hijo
 *	lee de la entrada estándar.
 *	Previamente, se ignora SIGINT, se captura SIGQUIT y se pone a por
 *	defecto SIGTERM.
 *	Luego podemos jugar a enviar estas señales antes y después del Exec.
 * Uso:
 *	FEW_signal mandato [argumentos...]
 */
#define MYNAME	"FEW_signal"

#define _XOPEN_SOURCE
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Handler(int signo)
{
	char * strsignal(int signo);

	printf(MYNAME": [%d] Recibida la señal \"%s\"(%d)\n",
		getpid(), strsignal(signo), signo);
	signal(signo, Handler);		/* <<=== Re-manejar señal */
}

int main(int argc, char * argv[])
{
	int pid, ret, status;
	char buff[80];

	if (argc == 1) {
		printf("USO: %s mandato [argumentos...]\n", MYNAME);
	} else {
		signal(SIGINT,  SIG_IGN);	/* <<=== Ignorar señal */
		signal(SIGQUIT, Handler);	/* <<=== Manejar señal */
		signal(SIGTERM, SIG_DFL);	/* <<=== Acción por defecto */

		switch(pid = fork()) {
		case -1:	/* Error */
			perror(MYNAME);
			break;
		case  0:	/* Hijo  */
			ret = read(0, buff, 80);
			fprintf(stderr, MYNAME": %d bytes leidos\n", ret);
			if (ret < 0) perror(MYNAME": En la llamada read()");
			execvp(argv[1], &argv[1]);
			perror(MYNAME);
			exit(1);
		default:	/* Padre */
			do {	/* Esperar a que termine este hijo. */
				ret = wait(&status);
				if (ret < 0) perror(MYNAME": En la llamada wait()");
			} while (ret != pid);
			printf(MYNAME": »»» [%d]%s: Estado=%d\n", ret, argv[1], status);
		}
	}
	return 0;
}
