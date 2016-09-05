/* Tamanyos.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira
 * Versión: 2.0		Fecha: 9 Sep 2009
 * Descripción:
 *	Muestra el tamaño de diversos tipo de datos
 *
 * Uso:
 *	Tamanyos
 */
#define MYNAME	"Tamanyos"

#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) 
{

	printf("\n\n                           Size %22s %22s\n", "Min", "Max");
	printf("----------------------------------------------------------------------------------\n");
        printf("sizeof(char):         %4d bytes %22d %22d \n", sizeof(char),CHAR_MIN,CHAR_MAX);
        printf("sizeof(short):        %4d bytes %22d %22d \n", sizeof(short),SHRT_MIN,SHRT_MAX);
        printf("sizeof(int):          %4d bytes %22d %22d \n", sizeof(int),INT_MIN,INT_MAX);
        printf("sizeof(long):         %4d bytes %22ld %22ld\n", sizeof(long),LONG_MIN,LONG_MAX);
        printf("------------------------------\n");
	printf("sizeof(long long):    %4d bytes\n", sizeof(long long));
        printf("sizeof(float):        %4d bytes\n", sizeof(float));
        printf("sizeof(double):       %4d bytes\n", sizeof(double));
        printf("sizeof(long double):  %4d bytes\n", sizeof(long double));
        printf("------------------------------\n");
        printf("sizeof(*int_ptr):     %4d bytes\n", sizeof(int *));

	return 0;
}
