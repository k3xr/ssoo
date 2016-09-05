/* CreaHueco.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 10 Nov 2005
 * Descripción:
 *	Se posiciona sobre la salida estándar en el offset indicado
 *	y escibe un caracter 'x'.
 *	Esta operación, dependiendo del offset, del contenido del fichero y del
 *	tipo de sistema de archivos, _puede_ crear un agujero en el fichero.
 *	El mandato "stat fichero" permite vel el tamaño y el número de bloques
 *	físicos (sectores de 512 bytes) de disco asignados al fichero.
 * Uso:
 *	CreaHueco offset >  fichero_creado_nuevo
 *	CreaHueco offset >> fichero_para_alterar
 */
#define MYNAME	"CreaHueco"

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	int flags;
	off_t offset;

	if (!argv[1] || 1 != sscanf(argv[1], "%lu", &offset)) {
		printf("Uso: "MYNAME" offset >  fichero_creado_nuevo\n");
		printf("Uso: "MYNAME" offset >> fichero_para_alterar\n");
	} else {
		/*
		 * Este programa puede haber sido ejecutado como:
		 * 	MYNAME offset >> fichero
		 * en cuyo caso estará activo el flag O_APPEND del descriptor 1.
		 * Si esto es así, toda escritura sucedería al final del fichero.
		 * Esto NO es lo que queremos... luego desactivamos este flag.
		 */
		flags = fcntl(1, F_GETFL);
		if (flags < 0)
			perror(MYNAME": fcntl(GETFL)");
		flags &= ~O_APPEND;	/* Ponemos a 0 este bit */
		if (fcntl(1, F_SETFL, flags) < 0)
			perror(MYNAME": fcntl(SETFL)");

		/* Nos posicionamos... */
		if (lseek(1, offset, SEEK_SET) < 0)
			perror(MYNAME": lseek(SET)");
		/* ...y escribimos. */
		if (write(1, "x", 1) < 0)
			perror(MYNAME": write(x)");
	}
	return 0;
}
