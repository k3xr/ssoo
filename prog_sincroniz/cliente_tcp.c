/* cliente_tcp.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: DATSI
 * Versión: 1.0		Fecha: 18 May 2010
 * Descripción:
 *	Ejemplo que ilustra la comunicación entre un cliente y un servidor
 *	utilizando el protocolo TCP.
 *
 * Uso:
 *	cliente localhost 
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

extern int h_errno;

int main(int argc, char * argv[])
{
   int cd;
   struct hostent * hp;
   struct sockaddr_in s_ain;
   unsigned char byte;
   int aux;
// Variables necesarias para getaddrinfo en lugar de gethostbyname
//   struct addrinfo hints;
//   struct addrinfo *result;

  if(argc<2 || !strcmp(argv[1],"--help"))
    {
      fprintf(stdout,
              "%s --help: Muestra la ayuda\n"
              "%s puerto_servidor\n",argv[0],argv[0]);
      return 0;
    }


/* Obtención del nombre de la máquina donde reside el servidor */
   fprintf(stdout,"CLIENTE:  Obtención del nombre donde reside el servidor: ");
   hp = gethostbyname(argv[1]); //Obsoleta; Sustituida por getaddrinfo

   if (hp == NULL){
	herror("gethostbyname");
	exit(1);
   }
   fprintf(stdout,"OK\n");

   bzero((char *)&s_ain, sizeof(s_ain));
   s_ain.sin_family = AF_INET;
   memcpy (&(s_ain.sin_addr), hp->h_addr, hp->h_length); /* IP */
   s_ain.sin_port = htons(24123); 

//   bzero((char *)&hints, sizeof(hints));
//   hints.ai_family = AF_INET;
//   hints.ai_socktype = SOCK_STREAM;
//   hints.ai_protocol = IPPROTO_TCP;
//   hints.ai_flags = 0;
//   hints.ai_canonname = NULL;
//   hints.ai_addr = NULL;
//   hints.ai_next = NULL;
//
//   aux = getaddrinfo(NULL,argv[1], hints, result);
//   if (aux !=0){
//   	perror("getaddrinfo");
//   	exit(1);
//   	}

/* Creacion del socket TCP */
   fprintf(stdout,"CLIENTE:  Creación del socket TCP: ");

   cd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (cd < 0){
	perror("socket");
	exit(1);
   }
   fprintf(stdout,"OK\n");


   fprintf(stdout,"CLIENTE:  Conexión al puerto del servidor: ");
   aux = connect(cd, (struct sockaddr *)&s_ain, sizeof(s_ain));
   if (aux < 0){
	perror("conexión fallida");
	close(cd);
	exit(1);
   }
   fprintf(stdout,"OK\n");


/* Lee un dato de la entrda, lo envía al servidor, espera su respuesta
 * y la escribe en la salida */
   while( (aux = read( 0, &byte, 1) == 1)) {
         send(cd, &byte, 1, 0);  /* Bloqueante */
         recv(cd, &byte, 1, 0);  /* Bloqueante */
         write(1, &byte, 1);
   }
   if (aux < 0){
	perror("read");
	exit(1);
   }
   fprintf(stdout,"CLIENTE:  Transferencia completada.\n");

   close(cd);
   return 0;
}
