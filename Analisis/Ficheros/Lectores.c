/* Lectores.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Modificado por: Luis Miguel Mazaira
 * Versión: 12.0		Fecha: 89 Sept 2009
 * Descripción:
 * Uso:
 *	Lectores
 */
#define MYNAME "Pipe9Lectores"

#include <sys/wait.h>
#include <ctype.h>
#include <sched.h>	/* sched_yield() */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int pp[2];
	int i;
	char ch;

	if (pipe(pp) < 0) {
		perror(MYNAME": pipe()");
		exit(1);
	}

	for (i = 1; i <= 9; i++) {
		switch (fork()) {
		case -1:
			perror(MYNAME": fork()");
			exit(1);
		case 0:
			close(pp[1]);
			while (1 == read(pp[0], &ch, 1)) {
				if (isdigit(ch)) ch += i;
				write(1, &ch, 1);
			}
			exit(0);
		}
	}

	close(pp[0]);
	i = 0;
	while (i < 20*50) {
		write(pp[1], "0", 1);
		/* Escoger el método de ceder la CPU */
		if (1)	sched_yield();
		else	usleep(1000*10);
		i++;
		if (i%50 == 0)
			write(pp[1], "\n", 1);
	}
	close(pp[1]);
	for (i = 1; i <= 9; i++)
		wait(NULL);
	return 0;
}
