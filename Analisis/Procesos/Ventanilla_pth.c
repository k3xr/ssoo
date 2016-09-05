/* Ventanilla_pth.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 28 Nov 2005
 * Descripción:
 * 	Este programa modela un servidor muy sencillo.
 * 	Para cada caracter introducido, se invoca a una función de servicio
 * 	que hace eco del caracter tras cierta cantidad de tiempo
 * 	dependiente de dicho caracter y con unos atributos dependientes del
 * 	número total de caracteres introducidos hasta el momento.
 *
 *	Esta versión de servidor arranca un hilo por petición.
 *	Existirá concurrencia y compartición.
 *	Cada caracter es procesado por separado y concurrentemente.
 *	La variable estática cnt se comparte, luego los atributos de
 *	presentación van cambiando.
 * Uso:
 * 	Ventanilla_pth
 */
#define MYNAME	"Ventanilla_pth"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ServiceFunction(unsigned char ch)
{
	unsigned char buff[] = "[__;4m_[0m";
	static unsigned cnt;
	/* La duración del servicio depende del parámetro */
	usleep((256-ch)*20000);
	/* Los atributos dependen del número de orden */
	buff[3] = '0'+ (cnt%8);
	buff[2] = "349"[cnt/8%3];
	cnt++;
	/* Finalmente muestra un resultado */
	buff[7] = ch;
	write(1,buff,12);
}

#include <pthread.h>
void ServiceFWrapper(unsigned char ch);
volatile int nthrds = 0;

int main(void)
{
	unsigned char ch;

	while(read(0,&ch,1) == 1)
	{
		ServiceFWrapper(ch);
	}
	while (nthrds > 0)
		usleep(20000);
	write(1,"\n",1);
	return 0;
}

void * ServiceFCaller(void * param)
{
	unsigned char ch = *(char *)param;
	ServiceFunction(ch);
	free(param);
	nthrds--;
	return NULL;
}

void ServiceFWrapper(unsigned char ch)
{
	char * param = malloc(1);
	pthread_t tid;
	*param = ch;
	pthread_create(&tid, NULL, ServiceFCaller, param);
	pthread_detach(tid);
	nthrds++;
}
