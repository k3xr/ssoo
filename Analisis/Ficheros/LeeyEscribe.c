/* LeeyEscribe.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Luis Miguel Mazaira 
 * Versión: 1.0		Fecha: 89 Sept  2009
 * Descripción:
 *	La información que lse lee de la entrada estándar.
 *      es volcada a un fichero especificado como argumento. 
 * Uso:
 *	LeeyEscribe Fichero
 */
#define MYNAME "LeeyEscribe"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
	int perm = 0600; // Lectura y Escritura
	int mode = O_CREAT|O_TRUNC|O_RDWR; //1102;  // O_CREATE|O_RDWR
	int fd,i;		  
	int cnt_Lect, cnt_Esc;
	char buff[512];

	if (argc!=2)
		printf("Uso: "MYNAME" archivo \n");
	else
	{
		fd = open(argv[1],mode,perm);
		if (fd<0)
			perror(MYNAME": open");
		else
		{
			while ((cnt_Lect = read(0, buff, 512)) > 0) 
			{
                                printf("LEIDOS: %d - ", cnt_Lect);
                                for (i = 0; i < cnt_Lect; i++)
                                {
                                        if (iscntrl(buff[i]))
                                                printf("%03o,", (int)buff[i]);
                                        else
                                                printf("'%c',", buff[i]);
                                }
                                printf("\n");

                        	cnt_Esc=write(fd, buff, cnt_Lect);
				if (cnt_Esc<0)
					perror(MYNAME);
                	}
			close(fd);
		}
	}
	return 0;
}
