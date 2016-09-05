/* Lincon.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *		NO FUNCIONA, ¿POR QUÉ?
 *
 * Uso:
 *	Lincon fichero...
 */
#define MYNAME	"Lincon"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define TAMBUF 65536

int total=0;

int incognita(char *arg)
{
        int fo, nbytes, i, valor=0;
        char buf[TAMBUF];

        if ((fo=open(arg, O_RDONLY))<0)
        {
                fprintf(stderr, MYNAME": Fichero %s: ", arg);
                perror("Error abriendo fichero");
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
	printf("%d\t%s\n", valor, arg);
        return 0;
}

int main(int argc, char *argv[])
{
	int i, estado;

	if (argc<2)
	{
		fprintf(stderr, "Uso: %s fichero ...\n", MYNAME);
		exit(1);
	}

	for (i=1; i<argc; i++)
		if (fork()==0)
		{
			estado=incognita(argv[i]);
			exit(estado);
		}
	for (i=1; i<argc; i++)
		wait(NULL);
	printf("Total:\t%d\n", total);
	exit(0);
}
