/* Volcado_mem.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0		Fecha: 4 Apr 2006
 * Descripción:
 *	Imprime el valor almacenado en un intervalo de direcciones de
 *	memoria. Distingue si la dirección está accesible o no, y, en
 *	caso afirmativo, muestra si es de sólo lectura o permite leer
 *	y escribir.
 *
 * Uso:
 *	Volcado_mem
 */	
#define MYNAME	"Volcado_mem"										// VM-01 
													// VM-02
#include <stdio.h>											// VM-03
#include <stdlib.h>											// VM-04
#include <pthread.h>											// VM-05
#include <unistd.h>											// VM-06
#include <signal.h>											// VM-07
													// VM-08
int cuenta;												// VM-09
int escritura=0;											// VM-10
char *dir_inicial;											// VM-11
pthread_t thid;												// VM-12
unsigned char aux;											// VM-13
													// VM-14
void captura(int sen) {											// VM-15
	if (escritura)											// VM-16
		printf("%p : %hhu (Solo lectura)\n", dir_inicial, aux);					// VM-17
	else												// VM-18
		printf("%p : NO ASIGNADA\n", dir_inicial);						// VM-19
	dir_inicial++; cuenta--;									// VM-20
	pthread_exit(NULL);										// VM-21
}													// VM-22
													// VM-23
void *recorre(void *arg){										// VM-24
	for ( ; cuenta; dir_inicial++, cuenta--) {							// VM-25
		escritura=0;										// VM-26
		aux=*dir_inicial;									// VM-27
		escritura=1;										// VM-28
		*dir_inicial=aux;									// VM-29
		printf("%p : %hhu (Lectura y escritura)\n", dir_inicial, aux);				// VM-30
	}												// VM-31
	return NULL;											// VM-32
}													// VM-33
int main(int argc, char *argv[]){									// VM-34
													// VM-35
	if (argc!=3) {											// VM-36
		fprintf(stderr, "Uso: %s dirección inicial (en hexadecimal) tamaño\n", MYNAME);		// VM-37
		return 1;										// VM-38
	}												// VM-39
													// VM-40
	signal(SIGSEGV, captura);									// VM-41
													// VM-42
	dir_inicial=(char *)strtoul(argv[1], (char **)NULL, 16);					// VM-43
	cuenta=atoi(argv[2]);										// VM-44
	while (cuenta) {										// VM-45
		pthread_create(&thid, NULL, recorre, NULL);						// VM-46
		pthread_join(thid, NULL);								// VM-47
	}												// VM-48
	return 0;											// VM-49
													// VM-51
}													// VM-52
