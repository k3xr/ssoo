/* Invocar_Exec.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Modificado por: Luis Miguel Mazaira
 * Versión: 1.0		Fecha: 29 Sep 20009
 * Descripción:
 *	Realiza la llamada exec para ejecutar el programa identificadores.
 * Uso:
 *	Invocar_Exec	
 */
#define MYNAME	"Invocar_Exec"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{

	printf(MYNAME": Antes  \tPID=%d\tPPID=%d\n", getpid(), getppid());
	execl("./Identificadores", "./Identificadores", NULL);
	printf(MYNAME": Antes  \tPID=%d\tPPID=%d\n", getpid(), getppid());
	exit(1);

	return 0;
}
