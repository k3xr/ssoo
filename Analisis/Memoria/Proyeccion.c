/* Proyeccion.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Permite al usuario especificar los parámetros del mmap de una
 *	proyección y ver el resultado de la misma.
 *
 * Uso:
 *	Proyeccion dir tam prot comp fija fichero desp
 */
#define MYNAME	"Proyeccion"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>


/* Función que imprime el mapa de memoria del proceso en ese instante */
static void mostrar_mapa(char *titulo){
        char mandato[256];
        int mipid;

	printf("\n----------- %s ------------\n", titulo);
	mipid= getpid();
        sprintf(mandato, "cat /proc/%d/maps", mipid);
        system(mandato);
	printf("-------------------------------------------------------------------\n");
}


int main(int argc, char **argv) {
	char *dir;
	char *dirini=NULL;
	int longitud, proteccion, desc, desp;
	int flags=0;

	
	if (argc!=8) {
		fprintf(stderr, "Uso: %s dir tam prot comp fija fichero desp\n", MYNAME);
		fprintf(stderr, "\tDebe especificar estos 7 argumentos:\n");
		fprintf(stderr, "\t1º Dirección de proyección en hexadecimal (0 si no se quiere especificar)\n");
		fprintf(stderr, "\t2º Tamaño de la proyección\n");
		fprintf(stderr, "\t3º Protección: 3 caracteres formato rwx (carácter - sin permiso)\n");
		fprintf(stderr, "\t4º Tipo de compartimiento: C o P\n");
		fprintf(stderr, "\t5º ¿Proyección fija (MAP_FIXED)?: S o N\n");
		fprintf(stderr, "\t6º Nombre del archivo (\"anon\" para tipo anónimo)\n");
		fprintf(stderr, "\t7º A partir de qué byte del archivo se proyecta\n");
		exit(1);
	}

	dirini=(void *)atol(argv[1]);
	dirini=(void *)strtoul(argv[1], (char **)NULL, 16);

	if (strcmp("anon", argv[6])){
		if ((desc=open(argv[6], O_RDWR))<0) {
			perror(MYNAME": Error abriendo archivo");
			exit(1);
		}
	}
	else {
		flags=MAP_ANON;
		desc=-1;
	}

	/* Obtengo desplazamiento */
	desp=atoi(argv[7]);

	/* Obtengo tamaño de la proyección */
	longitud=atoi(argv[2]);

	/* Obtengo si es fija o no */
	if ((argv[5][0]=='S') || (argv[5][0]=='s'))
		flags|=MAP_FIXED;
		
	/* Obtengo tipo de compartimiento */
	if (argv[4][0]=='C')
		flags|=MAP_SHARED;
	else if (argv[4][0]=='P')
		flags|=MAP_PRIVATE;
	else {
		fprintf(stderr, MYNAME": El tipo de compartimiento debe ser C o P\n");
		exit(1);
	}
		
	/* Obtengo los permisos */
	proteccion=0;
	if (argv[3][0]=='r')
		proteccion|=PROT_READ;
	if (argv[3][1]=='w')
		proteccion|=PROT_WRITE;
	if (argv[3][2]=='x')
		proteccion|=PROT_EXEC;

	/* Se muestra el mapa antes de llamar a mmap */
	mostrar_mapa("Antes de llamar a mmap");

	/* Se hace el mmap y se cierra el archivo */
	if ((dir=mmap(dirini, longitud, proteccion, flags, desc, desp))
			==(void *)-1) {
		perror(MYNAME": Error en mmap");
		close(desc);
		exit(1);
	}

	if (desc!=-1)
		close(desc);

	/* Se muestra el mapa después de llamar a mmap */
	mostrar_mapa("Después de llamar a mmap");

	munmap(dir, longitud);
	
	/* Se muestra el mapa después de llamar a munmap */
	mostrar_mapa("Después de llamar a munmap");

	return 0;
}

	

