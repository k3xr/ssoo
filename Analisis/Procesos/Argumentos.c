/* Argumentos.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Luis Miguel Mazaira 
 * Versión: 1.0		Fecha: 27 Sep 20049
 * Descripción:
 *	Muestra los argumentos de invocación
 * Uso:
 *	Argumentos [argumentos...]
 */
#define MYNAME	"Argumentos"

#include <stdio.h>

int main(int argc, char * argv[])
{
	int  num;

	printf(MYNAME": argc	=%d\n", argc);

	for (num=0; num < argc; num++)
		printf(MYNAME": argv[%d]	=%s\n", num, argv[num]);

	return 0;
}
