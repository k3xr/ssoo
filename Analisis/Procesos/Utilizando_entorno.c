/* Utilizando_Entorno.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politéica de Madrid, Facultad de Informáca
 *
 * Autor: Luis Miguel Mazaira
 * Versió1.0		Fecha: 19 9 9 9 9 9 9 9 9 Sept 20009
 * Descripció * 	E
 * Uso:
 *	Utilizando_Entorno
 */
#define MYNAME	"Utilizando_Entorno"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *var;
        if ((var=getenv("SEMILLA")))
                        srand(atoi(var));
 
	printf("\n\t[%d] - [%d]\n\n",rand()%6+1, rand()%6+1);

	return 0;
}
