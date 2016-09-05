/* CutandPaste.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 10 Nov 2005
 * Descripción:
 *	Ejemplo que ilustra el buffering de I/O con las funciones
 *	fgets/fputs 
 *	Lee por la entrada est�ndar un valor num�rico y un string y 
 *	los escribe en la salida est�ndar o en un fichero.
 *	La escritura se realiza de dos formas alternativas que permiten
 *	comprobar el funcionamiento del buffering de entrada/salida.
 * Uso:
 *	buffering fichin1 fichin2 
 *	buffering --help 
 */
#define MYNAME	"buffering"

#define BUFF_SIZE 4096

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
	FILE *fd_in, *fd_out;
	size_t size;
//	off_t offin, offout;
	char auxchar;
	char linea[LINE_MAX];
	int num_bytes;
	char auxstr[10];

/* Comprobamos argumentos */
	if ((argc < 3) || (strcmp(argv[1],"--help")==0))
	{
		num_bytes=fprintf(stdout, MYNAME" --help: Muestra la ayuda\n");
		num_bytes=fprintf(stdout, "Uso: "MYNAME" fichin fichout\n"); 
		return 0;
	}

/* Abrimos los ficheros */
	if (strcmp(argv[1],"-")==0)
	{
		fd_in = fopen("datos.txt","a+");
		if (fd_in == NULL)
		{
			num_bytes=fprintf(stderr,MYNAME": fopen(%s):%s\n","stdin",strerror(errno));
			return 1;
		}
	}
	else if ((fd_in=fopen(argv[1],"w+"))==NULL)
	{
		num_bytes=fprintf(stderr,MYNAME": fopen(%s):%s\n",argv[1],strerror(errno));
		return 1;
	}

/* El de salida */
	if (strcmp(argv[2],"-")==0)
	{
		fd_out = freopen("salida2","w+",stdout);
		if (fd_out == NULL)
		{
			num_bytes=fprintf(fd_out,MYNAME": fopen(%s):%s\n","salida2",strerror(errno));
			return 1;
		}
	}
	else if ((fd_out=fopen(argv[2],"w+"))==NULL)
	{
		num_bytes=fprintf(fd_out,MYNAME": fopen(%s):%s\n",argv[2],strerror(errno));
		return 1;
	}

/* Leemos los datos */

/* C�digo incorrecto */
	num_bytes=printf("Introduce el valor entero a leer: ");
	num_bytes=scanf("%d", &size);
	num_bytes=printf("Introduce una linea de texto: ");
	num_bytes=strlen(fgets(linea, LINE_MAX, stdin));
	sprintf(auxstr,"%d",size);
	fputs(auxstr,fd_in);
	fputs(linea,fd_in);
/* Fin de c�digo incorrecto */

/* C�digo correcto */
	num_bytes=printf("Introduce el valor entero a leer: ");
	scanf("%d", &size);
	while( (auxchar = fgetc(stdin)) != EOF && auxchar != '\n' )
	{
		if ((auxchar != EOF) && (auxchar != '\n'))
		{
			num_bytes=fprintf(stderr,MYNAME": fopen(%s):%s\n",argv[1],strerror(errno));
			return 1;
		}
	}
	sprintf(auxstr,"%d",size);
	fputs(auxstr,fd_out);
	puts(auxstr);
	num_bytes=printf("Introduce una linea de texto: ");
	while (fgets(linea, LINE_MAX, stdin) != NULL)
	{};
	fputs(linea,fd_out);
	puts(linea);
/* Fin de c�digo correcto */

	fclose(fd_in);
	fclose(fd_out);
	return 0;

}
