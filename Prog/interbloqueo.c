/* interbloqueo.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: DATSI
 * Versión: 1.0		Fecha: 18 May 2010
 * Descripción:
 *	Ejemplo que ilustra la creación de un interbloqueo.
 *      Hay un problema de carreras entre dos threads que acceden a
 *      una zona común mediante mutex.
 * Uso:
 *	interbloqueo 
 *
 * Comentario: 
 * 	Montar con la librería pthread
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t primer_mutex;
pthread_mutex_t segundo_mutex;

void *funcion_primera(void *agumentos); 
void *funcion_segunda(void *agumentos); 

int aux=0; /* Tratamiento de errores */

int main(int argc, char *argv[])
{
pthread_t tid1, tid2; /* Identificadores de los threads */
pthread_attr_t attr; /* Atributos de los threads */


if((argc == 2) && !strcmp(argv[1],"--help"))
{
    fprintf(stdout,
              "%s --help: Muestra la ayuda\n"
              "%s \n",argv[0],argv[0]);
    return 0;
}


/* Inicialización de los atributos de los threads con los valores por defecto */
aux=pthread_attr_init(&attr);
if (aux != 0)
    {
      perror("Error en la función pthread_attr_init");
      return 1;
    }

/* Creamos los mutex */
aux=pthread_mutex_init(&primer_mutex,NULL);
if (aux != 0)
    {
      perror("Error en la creación del primer mutex con pthread_mutex_init");
      return 1;
    }
aux=pthread_mutex_init(&segundo_mutex,NULL);
if (aux != 0)
    {
      perror("Error en la creación del segundo mutex con pthread_mutex_init");
      return 1;
    }

/* Creamos los threads */
aux=pthread_create(&tid1,&attr,funcion_primera,NULL);
if (aux != 0)
    {
      perror("Error en la creación del primer thread con pthread_create");
      return 1;
    }
aux=pthread_create(&tid2,&attr,funcion_segunda,NULL);
if (aux != 0)
    {
      perror("Error en la creación del segundo thread con pthread_create");
      return 1;
    }

/* Barreras para ambos threads */
aux=pthread_join(tid1,NULL);
if (aux != 0)
    {
      perror("Error en la barrera del primer thread con pthread_join");
      return 1;
    }
aux=pthread_join(tid2,NULL);
if (aux != 0)
    {
      perror("Error en la barrera del segundo thread con pthread_join");
      return 1;
    }

printf("El padre de los threads que tiene PID %d ha concluido su espera\n",(int) getpid());

/* Liberamos los mutex */
aux=pthread_mutex_destroy(&primer_mutex);
if (aux != 0)
    {
      perror("Error en la liberación del primer mutex con pthread_mutex_destroy");
      return 1;
    }
aux=pthread_mutex_destroy(&segundo_mutex);
if (aux != 0)
    {
      perror("Error en la liberación del segundo mutex con pthread_mutex_destroy");
      return 1;
    }
return 0;
}

void *funcion_primera(void *agumentos) 
{

/* En el bucle se espera que se produzca en algún momento alguna condición
 * de carrera entre los dos threads que provoque su interbloqueo */

while(1) {
aux=pthread_mutex_lock(&primer_mutex);
if (aux != 0)
    {
      perror("Error en el cierre del primer mutex con pthread_mutex_lock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }

/* Testigo para seguir la traza de ejecución de los threads */

fprintf(stderr,"Thread invocado desde la función 1");
aux=pthread_mutex_lock(&segundo_mutex);
if (aux != 0)
    {
      perror("Error en el cierre del segundo mutex con pthread_mutex_lock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
/**
 * Se realiza alguna acción
 */
aux=(int) pthread_self();
fprintf(stdout,"Identificador del thread de la primera función: %d\n",aux);

aux=pthread_mutex_unlock(&segundo_mutex); 
if (aux != 0)
    {
      perror("Error al liberar el segundo mutex con pthread_mutex_unlock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
aux=pthread_mutex_unlock(&primer_mutex); 
if (aux != 0)
    {
      perror("Error al liberar el primer mutex con pthread_mutex_unlock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
}
pthread_exit(0);
}

void *funcion_segunda(void *agumentos)
{

/* En el bucle se espera que se produzca en algún momento alguna condición
 * de carrera entre los dos threads que provoque su interbloqueo */

while(1) {
aux=pthread_mutex_lock(&segundo_mutex);
if (aux != 0)
    {
      perror("Error en el cierre del segundo mutex con pthread_mutex_lock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
/* Testigo para seguir la traza de ejecución de los threads */

fprintf(stderr,"Thread invocado desde la función 2");
aux=pthread_mutex_lock(&primer_mutex);
if (aux != 0)
    {
      perror("Error en el cierre del primer mutex con pthread_mutex_lock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
/**
* Se realiza alguna acción
*
*/ 
         
aux=(int) pthread_self();
fprintf(stdout,"Identificador del thread de la segunda función: %d\n",aux);
         
aux=pthread_mutex_unlock(&primer_mutex);
if (aux != 0)
    {
      perror("Error al liberar el primer mutex con pthread_mutex_unlock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
aux=pthread_mutex_unlock(&segundo_mutex);
if (aux != 0)
    {
      perror("Error al liberar el segundo mutex con pthread_mutex_unlock");
      fprintf(stderr,"Desde el thread %d\n",(int) pthread_self());
      return 1;
    }
}
pthread_exit(0);
}

