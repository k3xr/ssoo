/* Lincon_th.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *
 * Uso:
 *      Lincon_th fichero...
 *
 *      Se debe compilar así: cc -Wall Lincon_th.c -o Lincon_th -lpthread
 */
#define MYNAME	"Lincon_th"


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

#define TAMBUF 65536
#define MAXARGS 256

int total=0;

int incognita(char *arg)
{
	int fo, nbytes, i, valor=0;
	char buf[TAMBUF];

	if ((fo=open(arg, O_RDONLY))<0)
	{
		fprintf(stderr, MYNAME": Fichero %s: ", arg);
		perror("Error abriendo fichero origen");
		return 1;
	}
	while ( (nbytes= read(fo, buf, TAMBUF)) >0)
		for (i=0; i<nbytes; i++)
			if (buf[i]=='\n') valor++;

	if (nbytes<0)
	{
		fprintf(stderr, MYNAME": Fichero %s: ", arg);
		perror("Error leyendo en fichero");
		return 2;
	}
	total+= valor;
	printf("%d %s\n", valor, arg);
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t  thid[MAXARGS];

	if (argc<2)
        {
                fprintf(stderr, "Uso: %s fichero ...\n", MYNAME);
                exit(1);
        }
	for (i=1; i<argc; i++)
		if (pthread_create(&thid[i], NULL, (void * (*)(void *))incognita,
			(void *)argv[i])!=0)
		{
			perror(MYNAME": Error creando thread");
			exit (1);
		}
	for (i=1; i<argc; i++)
		pthread_join(thid[i], NULL);
		
	printf("%d %s\n", total, "total");
	exit(0);
}
