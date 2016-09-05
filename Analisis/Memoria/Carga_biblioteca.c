/* Carga_biblioteca.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Muestra el mecanismo de carga explícita de bibliotecas
 *	dinámicas.
 *
 * Uso:
 *	Carga_biblioteca
 *
 * Compilación:
 *	cc -m32 -Wall -o Carga_biblioteca Carga_biblioteca.c -ldl
 *
 */
#define MYNAME	"Carga_biblioteca"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_mapa(char *titulo)
{
        char mandato[256];
        int mipid;

	printf("\n----------- %s ------------\n", titulo);
	mipid= getpid();
        sprintf(mandato, "pmap %d", mipid);
        system(mandato);
	printf("-------------------------------------------------------------------\n");
}


int main(int argc, char **argv)
{
	double valor;
	void *descriptor_bib;
	double (*fcos) (double);

	if (argc!=2) {
		fprintf(stderr, "Uso: %s argumento_coseno\n", MYNAME);
		exit(1);
	}

	/* Se muestra el mapa antes de cargar la biblioteca dinámica */
	mostrar_mapa("Antes de llamar a dlopen");
	
	/* Se carga la biblioteca matemática */
	if (!(descriptor_bib=dlopen("/usr/lib/libm.so", RTLD_LAZY))) {
		fprintf(stderr, MYNAME": %s\n", dlerror());
		return(1);
	}

	/* Se muestra el mapa antes de cargar la biblioteca dinámica */
	mostrar_mapa("Después de llamar a dlopen");
	
	/* Busca el símbolo correspondiente al coseno */
	if (!(fcos=dlsym(descriptor_bib, "cos"))) {
		fprintf(stderr, MYNAME": %s\n", dlerror());
		return(1);
	}

	valor=atof(argv[1]);
	printf("Coseno(%f)=%f\n", valor, fcos(valor));

	/* Se descarga la biblioteca dinámica */
	dlclose(descriptor_bib);

	/* Se muestra el mapa después de descargar la biblioteca dinámica */
	mostrar_mapa("Después de llamar a dlclose");
	
	return 0;
}

