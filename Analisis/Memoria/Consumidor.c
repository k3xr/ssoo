/* Consumidor.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Este programa está pensado para que see ejecute en combinación
 *	con el programa "Productor" conectados por una tubería.
 *
 * Uso:
 *	Productor | Consumidor
 */
#define MYNAME	"Consumidor"


#include <stdio.h>
#include <unistd.h>


struct datos {
	char * nombre;
	int puntuacion;
};
	
int main(void)
{
	struct datos undato;

	read(0, &undato, sizeof(undato));

	fprintf(stderr, "CONSUMIDOR: \n");
	printf("\tPuntuación %d\n", undato.puntuacion);
	printf("\tNombre %s\n", undato.nombre);

	return 0;
}
