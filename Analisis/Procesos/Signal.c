/* Signal_C.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 20 Sep 2004
 * Descripción:
 *	Programa que utiliza signal para ignorar la señal INT,
 *	capturar QUIT y poner TERM a "por defecto".
 *	El manejador re-captura la señal. Luego se para con un pause.
 *	Se puede ver su comportamiento frente a estas señales.
 * Uso:
 *	Signal_C
 */
#define MYNAME	"Signal_C"

#define _XOPEN_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void Handler(int signo)
{
	char * strsignal(int signo);

	printf(MYNAME": [%d] Recibida la señal \"%s\"(%d)\n",
		getpid(), strsignal(signo), signo);
	signal(signo, Handler);		/* <<=== Re-manejar señal */
}

int main(void)
{
	signal(SIGINT,  SIG_IGN);	/* <<=== Ignorar señal */
	signal(SIGQUIT, Handler);	/* <<=== Manejar señal */
	signal(SIGTERM, SIG_DFL);	/* <<=== Acción por defecto */

	while (1)
		pause();
	return 0;
}
