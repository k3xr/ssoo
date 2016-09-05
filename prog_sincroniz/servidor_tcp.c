/* servidor_tcp.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: DATSI
 * Versión: 1.0		Fecha: 18 May 2010
 * Descripción:
 *	Ejemplo que ilustra la creación un servidor con comunicación siguiendo
 *	el protocolo TCP.
 * Uso:
 *	servidor & 
 *
 */

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

extern int h_errno;

int main(void)
{
   int sd, cd, size;
   unsigned char byte;
   struct sockaddr_in s_ain, c_ain;
   int aux;

   /* Creacion del socket TCP */
   fprintf(stdout,"SERVIDOR: Creacion del socket TCP: ");
   sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sd < 0){
	perror("socket");
	exit(1);
   }
   fprintf(stdout,"OK\n");


   bzero((char *)&s_ain, sizeof(s_ain));
   s_ain.sin_family = AF_INET;
   s_ain.sin_addr.s_addr = INADDR_ANY; /*Cualquier origen*/
   s_ain.sin_port = htons(24123); 

   fprintf(stdout,"SERVIDOR: Asignación del puerto en el servidor: ");
   aux = bind(sd, (struct sockaddr *)&s_ain, sizeof(s_ain));
   if (aux < 0){
	perror("bind");
        close(sd);
	exit(1);
   }
   fprintf(stdout,"OK\n");
   
   /* Aceptamos conexiones por el socket */
   fprintf(stdout,"SERVIDOR: Aceptacion de peticiones: ");
   aux = listen(sd, 5);   /* 5 = tamaño cola */
   if (aux < 0){
	perror("listen");
        close(sd);
	exit(1);
   }
   fprintf(stdout,"OK\n");

   while(1) {
      size = sizeof(c_ain);
      /* Esperamos la llegada de una conexion */
      fprintf(stdout,"SERVIDOR: Llegada de un mensaje: ");
      cd = accept(sd, (struct sockaddr *)&c_ain, &size);
      if (cd < 0){
	perror("accept");
        close(sd);
	exit(1);
      }
      fprintf(stdout,"OK\n");

      /* Se crea una réplica para atender el servicio */
      switch(fork()) {
      case -1:
         perror("error en la replicación del servidor");
         return 1;
      case 0:
         close(sd);
         /* El servidor recibe y se envía mensajes */
         while(recv(cd, &byte, 1, 0) == 1) /*Bloqueante*/
            send(cd, &byte, 1, 0);	   /*Bloqueante*/
         close(cd);
         return 0;
      default:
         close(cd);
      } /* switch */
   } /* while */
} /* main */
