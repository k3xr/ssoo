/* Cada3Segs.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 15 Oct 2005
 * Modificado por: Luis Miguel Mazaira
 * Versión: 1.1		Fecha: 19 Sep 2009
 * Descripción:
 * 	El proceso captura la señal de alarma y se lanza una
 * 	cada 3 segundos.
 *      En el tratamiento de la alarma no se realiza ninguna accion.
 *      Para ver que efectivamente se trta una alarma cada 3segs, 
 *      solo es necesario eliminar el comentario del printf  en la 
 *      funcion de tratar_alarma
 * Uso:
 *	Cada3Segs
 */
#define MYNAME	"Cada3Segs"

#define _GNU_SOURCE
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void tratar_alarma(int signo)
{
	//printf(MYNAME": Señal %s activada\n", strsignal(signo));
}

int main(void)
{
	struct sigaction act;

	/* establece el manejador para SIGALRM */
	act.sa_handler = tratar_alarma;	/* función a ejecutar */
	act.sa_flags = 0;		/* ninguna opción especifica */
	sigaction(SIGALRM, &act, NULL);
	act.sa_handler = SIG_IGN;	/* ignora la señal SIGINT */
	sigaction(SIGINT, &act, NULL);
	for (;;) {			/* recibe SIGALRM cada 3 segundos */
		alarm(3);
		pause();
	}
	return 0;
}
