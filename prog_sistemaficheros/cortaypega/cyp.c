/* CutandPaste.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Polit√©cnica de Madrid, Facultad de Inform√°tica
 *
 * Autor: Francisco Rosales
 * Versi√≥n: 1.0		Fecha: 10 Nov 2005
 * Descripci√≥n:
 *	Se posiciona sobre el fichero de entrada en el offset indicado
 *	y escribe sobre el fichero de salida, y en la posici√≥n 
 *	correspondiente al offset, la cantidad de bytes indicado como
 *	argumento 
 * Uso:
 *	cutandpaste fichin offset1 cantidad fichout offset2 
 *	cutandpaste --help 
 */
#define MYNAME	"cutandpaste"

#define BUFF_SIZE 4096

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int cutandpaste(int fdin, off_t offsetin, size_t size, int fdout, off_t offsetout)
{
	char buffer[BUFF_SIZE];
	size_t bytes, wbytes;

/* Nos posicionamos en el fichero de entrada... */
	if ((offsetin != 0) && lseek(fdin, offsetin, SEEK_SET) < 0)
	{
		fprintf(stderr,MYNAME": lseek(SET)(%d):%s\n",(int)offsetin,strerror(errno));
		return 1;
	}
/* ...en el fichero de salida... */
	if ((offsetout != 0) && lseek(fdout, offsetout, SEEK_SET) < 0)
	{
		fprintf(stderr,MYNAME": lseek(SET)(%d):%s\n",(int)offsetout,strerror(errno));
		return 1;
	}

/* ...leemos...*/
	while((bytes=read(fdin,buffer,min(size,BUFF_SIZE)))>0)
	{
/* ...y escribimos. */
		wbytes=write(fdout, buffer, bytes);
		if (wbytes < 0)
		{
			fprintf(stderr,MYNAME": write():%s\n",strerror(errno));
			return 1;
		}
		else 
		{
			size -= bytes;
		}
	}
	if (bytes < 0)
	{
		fprintf(stderr,MYNAME": read():%s\n",strerror(errno));
		return 1;
	}
	return 0;
}

int main(int argc, char * argv[])
{
	int fd_orig, fd_dest;
	size_t size;
	off_t offin, offout;
	char * end = NULL;

/* Comprobamos argumentos */
	if ((argc < 6) || (strcmp(argv[1],"--help")==0))
	{
		fprintf(stdout, MYNAME" --help: Muestra la ayuda\n");
		fprintf(stdout, "Uso: "MYNAME" fichin offset1 cantidad fichout offset2\n"); 
		return 0;
	}

/* Abrimos los ficheros */
	if (strcmp(argv[1],"-")==0)
	{
		fd_orig = 0;
	}
	else if ((fd_orig=open(argv[1],O_RDONLY))<0)
	{
		fprintf(stderr,MYNAME": open(%s):%s\n",argv[1],strerror(errno));
		return 1;
	}

/* El de salida */
	if (strcmp(argv[4],"-")==0)
	{
		fd_dest = 1;
	}
	if ((fd_dest=open(argv[4],O_CREAT|O_WRONLY,0666))<0) 
	{
		fprintf(stderr,MYNAME": creat(%s):%s\n",argv[4],strerror(errno));
		return 1;
	}

/* Leemos las cantidades num√ricas */

/* Offset de entrada */
	end = NULL;
	offin = strtol(argv[2],&end,10);
	if (!end || *end!='\0')
	{
		fprintf(stderr,MYNAME": offin(%s):No es un n√mero v√lido\n",argv[2]);
		return 1;
	}


/* Offset de salida */
	end = NULL;
	offout = strtol(argv[5],&end,10);
	if (!end || *end!='\0')
	{
		fprintf(stderr,MYNAME": offout(%s):No es un n√mero v√lido\n",argv[5]);
		return 1;
	}


/* Cantidad a copiar */
	end = NULL;
	size = strtol(argv[3],&end,10);
	if (!end || *end!='\0')
	{
		fprintf(stderr,MYNAME": size(%s):No es un n√mero v√lido\n",argv[3]);
		return 1;
	}


/* Realizamos la copia */
	return cutandpaste(fd_orig,offin,size,fd_dest,offout);
}
