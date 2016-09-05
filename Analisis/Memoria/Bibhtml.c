/* Bibhtml.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *      Módulo hipotético que procesase archivos HTML.
 *      Este fichero se compilará como una biblioteca y se usará
 *	desde el programa Navegador.
 *
 * Uso:
 *      Lo usa el programa Navegador
 *
 *	Compilación como biblioteca dinámica:
 *		cc -fPIC -shared -o Bibhtml Bibhtml.c
 *
 */
#define MYNAME	"Bibhtml"

#include <stdio.h>

void procesar(char *archivo)
{
	printf("Soy la biblioteca bibhtml y estoy procesando %s\n",
		archivo);
}
