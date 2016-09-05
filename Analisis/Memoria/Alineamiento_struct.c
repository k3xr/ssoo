/* Alineamiento_struct.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Modificado por: Luis Miguel Mazaira
 * Versión: 1.1		Fecha: 9 Sep 2009
 * Descripción:
 *	Muestra cómo cada campo de una estructura se almacena en una dirección
 *	que respeta las restricciones de alineamiento. Nótese que esto puede
 *	provocar "huecos" (zonas no usadas en la memoria)
 *
 * Uso:
 *	Alineamiento_struct
 */
#define MYNAME	"Alineamiento_struct"

#include <stdio.h>

 #include <stddef.h>
struct tipo {
	short s;
	int i;
	char c;
};


int main(int argc, char *argv[])
{
	struct tipo var;

	printf("Tamaño        %d\n", sizeof(struct tipo));
	printf("campo short s:  dir %p\n", &var.s);
	printf("campo int i:    dir %p\n", &var.i);
	printf("campo char c:   dir %p\n", &var.c);


        printf("sizeof(struct var)=%ld\n", (long) sizeof(struct tipo));
	printf("Memory Address var:   dir %p\n", &var);
        printf("Memory Address field s: %p -> %ld\n", &var.s, (long) offsetof(struct tipo, s));
        printf("Memory Address field i: %p -> %ld\n", &var.i, (long) offsetof(struct tipo, i));
        printf("Memory Address field c: %p -> %ld\n", &var.c, (long) offsetof(struct tipo, c));

	return 0;
}
