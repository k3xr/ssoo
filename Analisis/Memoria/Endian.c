/* Endian.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Fernando Pérez Costoya
 * Versión: 1.0
 * Modificado por: Luis Miguel Mazaira
 * Descripción:
 *	Muestra si el procesador usa un formato big-endian o little-endian
 *
 * Uso:
 *	Endian
 */
#define MYNAME	"Endian"
#include <string.h>
#include <stdio.h>

char * dec_to_bin(int number) {

	static char b[] = {'\0'};    
	int x, y;
	x = y = 0;
	
	b[0]='\0';

	for(y = 7; y >= 0; y--) 
	{
		x = number / (1 << y);
		number = number - x * (1 << y);
		strcat(b,x== 1 ?"1":"0");
 	}
	return b;
}


int main(int argc, char **argv) 
{
	int n=1;
	char *p;

	p=(char *)&n;
	printf("Enter an int value (0-256): ");
	scanf("%d", &n);
	if((n >= 0) && (n < 256))
	{	
		printf("\nsize of (int): %d bytes\n", sizeof(n));
		printf("Binary representation : 00000000-00000000-00000000-%s \n", dec_to_bin(n)) ;
		printf("------------------------------");
		printf("Memory allocation");
		printf("------------------------------\n");
		printf("Memory Addres: %p\t -> %s\n", p,dec_to_bin(*p));
		printf("Memory Addres: %p\t -> %s\n", p+1, dec_to_bin(*(p+1)));
		printf("Memory Addres: %p\t -> %s\n", p+2, dec_to_bin(*(p+2)));
		printf("Memory Addres: %p\t -> %s\n", p+3, dec_to_bin(*(p+3)));
	}
	else
		printf("The value must be in the range (0-256)\n");

	return 0;
}
