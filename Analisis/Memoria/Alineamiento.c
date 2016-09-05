/* Alineamiento.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira
 * Versión: 1.1		Fecha: 9 Sep 2009
 * Descripción:
 *	Muestra cómo cada variable se almacena en una dirección de manera que
 *	se respeten las restricciones de alineamiento. Nótese que esto puede
 *	provocar "huecos" (zonas no usadas en la memoria)
 *
 * Uso:
 *	Alineamiento
 */
#define MYNAME	"Alineamiento"

#include <stdio.h>

int main(int argc, char *argv[])
{
	int i1;
	short int s1;
	char c2;
	short int s2;
	char c1;
	short int s3;
	int i2;

	printf("int i2:   dir %p\n", &i2);
	printf("short s3: dir %p\n", &s3);
	printf("char c1:  dir %p\n", &c1);
	printf("short s2: dir %p\n", &s2);
	printf("char c2:  dir %p\n", &c2);
	printf("short s1: dir %p\n", &s1);
	printf("int i1:   dir %p\n", &i1);
	printf("Total         %d\n", (char *)&i1-(char*)(&i2-1));
	return 0;
}
