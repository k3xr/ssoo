/* Navegador.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Navegador: Averigua en tiempo de ejecución qué biblioteca debe usar
 *	para procesar un archivo dependiendo de la extensión de su nombre.
 *      Usa la técnica de la carga explícita de bibliotecas dinámicas
 *
 * Uso:
 *      Navegador
 *
 * Compilación:
 *	cc -Wall -o Navegador Navegador.c -ldl
 *
 */
#define MYNAME	"Navegador"

#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

/* Busca en el archivo de configuración qué biblioteca está asociada
   a la extensión especificada. Devuelve NULL si no la encuentra */

static char *buscar_biblio_en_config(char *extension_buscada) {
	FILE *fconfig;
	char *biblio;
	char extension_config[32];

	biblio=malloc(128*sizeof(char));

	if ((fconfig=fopen("config", "r"))==NULL) {
		perror(MYNAME": No puede abrir archivo de configuración");
		exit(1);
	}
	while (fscanf(fconfig, "%s %s", extension_config, biblio)!=EOF) 
		if (strcmp(extension_config, extension_buscada)==0) {
			fclose(fconfig);
			return biblio;
		}

	fclose(fconfig);
	return NULL;
}

/* Dado un nombre de archivo devuelve su extensión. Retorna NULL si el
   nombre no tiene extensión o el punto es el último carácter. */
static char *obtener_extension(char *archivo) {
		char *ext;

		ext=rindex(archivo, '.');
		/* Si hay un '.', ext != NULL */
		if (ext!=NULL) {
			/* se avanza después del punto */
			ext++;
			/* Si '.' es último car., no hay realmente extensión */
			if (*ext=='\0')
				ext=NULL;
		}
		return ext;
}

/* Procesa el archivo usando la biblioteca dinámica especificada */
static void tratar_archivo(char *archivo, char *biblioteca){
	void *descriptor_bib;
	void (*procesar)(char *);

	/* Se carga la biblioteca dinámica */
	if (!(descriptor_bib=dlopen(biblioteca, RTLD_LAZY))) {
		fprintf(stderr, MYNAME": Error cargando biblioteca: %s\n", dlerror());
		return;
	}

	/* Busca el símbolo correspondiente a "procesar" */
	if (!(procesar=dlsym(descriptor_bib, "procesar")))
		/* La función no existe */
                fprintf(stderr, MYNAME": Error: biblioteca no incluye función procesar\n");
	else
		/* Finalmente, procesamos el archivo */
		procesar(archivo);

	/* Se descarga la biblioteca */
	dlclose(descriptor_bib);
}

int main() {
	char archivo[256];
	char *extension;
	char *biblioteca;

	printf("Introduzca el nombre del archivo que quiere procesar: ");
	while (scanf("%s", archivo)!=EOF) {
		/* Obtiene la extensión correspondiente al nombre */		
		extension=obtener_extension(archivo);
		if (extension==NULL)
			fprintf(stderr, MYNAME": Error: el nombre de archivo debe tener extensión\n");
		else {
			/* Busca la bobliteca asociada */
			biblioteca=buscar_biblio_en_config(extension);
			if (biblioteca==NULL)
				fprintf(stderr, MYNAME": Error: extensión no configurada\n");
			else
				/* procesa el archivo usando la bibloteca */
				tratar_archivo(archivo, biblioteca);
		}

		printf("Introduzca el nombre del archivo que quiere procesar: ");
	}
	return 0;
}
