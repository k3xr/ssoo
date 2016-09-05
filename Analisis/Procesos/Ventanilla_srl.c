/* Ventanilla_srl.c */
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
 *	Esta versión de servidor usa un único proceso para todo.
 *	No existirá concurrencia pero si compartición.
 *	Los caracteres se procesan uno a uno secuencialmente.
 *	La variable estática cnt se comparte, luego los atributos de
 *	presentación van cambiando.
 * Uso:
 * 	Ventanilla_srl
 */
#define MYNAME	"Ventanilla_srl"

#include <stdio.h>
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




int main(void)
{
	unsigned char ch;

	while(read(0,&ch,1) == 1)
	{
		ServiceFunction(ch);
	}
	write(1,"\n",1);
	return 0;
}
