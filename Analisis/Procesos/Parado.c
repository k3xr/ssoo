/* Parado.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 20 Sep 2004
 * Descripción:
 *	Programa que se para.
 *	La única manera de hacerlo terminar es enviarle una señal que lo mate.
 * Uso:
 *	Parado
 */
#define MYNAME	"Parado"

#include <unistd.h>

int main(void)
{
	pause();
	return 0;
}
