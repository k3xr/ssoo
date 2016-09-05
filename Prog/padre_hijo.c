/* Padre_hijo.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: DATSI
 * Versión: 1.0		Fecha: 18 May 2010
 * Descripción:
 *	Ejemplo que ilustra la creación de procesos padre/hijo para ser 
 *	utilizado como demo en una sesión de depuración de este tipo de 
 *	programas.
 * Uso:
 *	padre_hijo ejecutable [argumentos...]
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int parate=0;

int main(int argc, char * argv[])
{
pid_t pid;

  if(argc<2 || !strcmp(argv[1],"--help"))
    {
      fprintf(stdout,
              "%s --help: Muestra la ayuda\n"
              "%s ejecutable [argumentos...]\n",argv[0],argv[0]);
      return 0;
    }


   /* Creamos el proceso hijo */
   pid = fork(); 
   if (pid < 0) { /* Error en el fork*/
	perror("Error al crear el hijo");
	return 1;
   }
   else if (pid == 0) { /* Proceso hijo*/
/* La siguiente línea es el código que hay que incorporar para 
 * el depurador */
   	while (parate==1) continue; 
	printf("Soy el hijo %d\n",pid);
	printf("PID real del hijo impreso desde el hijo %d\n",getpid());
	printf("PID real del padre impreso desde el hijo %d\n",getppid());
	execvp(argv[1],&(argv[1]));
	perror("Error al ejecutar el mandato desde el hijo");
	return 1;
   }
   else { /* Proceso padre*/
   /* Espera a que termine el hijo */
	printf("Soy el padre %d\n",getpid());
	printf("Soy el padre imprimiendo el pid del hijo %d\n",pid);
	pid=wait(NULL);
	printf("El hijo con PID %d ha terminado\n",pid);
	return 0;
   }
}
