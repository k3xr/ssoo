/* UDP_s.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Servidor de eco (puerto 7), para Clientes remotos.
 *	Comunicación vía sockets UDP.
 * Uso:
 *	UDP_s
 */
#define MYNAME	"UDP_s"

#include <arpa/inet.h>			/* inet_* */
#include <netinet/in.h>			/* IP*, sockaddr_in, ntoh?, hton?, etc. */
#include <netinet/tcp.h>		/* TCPOPT_*, etc. */
#include <sys/socket.h>			/* socket */
#include <sys/types.h>			/* socket */
#include <sys/un.h>			/* sockaddr_un */
#include <netdb.h>			/* gethostbyname */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int sd, size;
	unsigned char byte;
	struct sockaddr_in s_ain, c_ain;

	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	bzero((char *) &s_ain, sizeof(s_ain));
	s_ain.sin_family = AF_INET;
	s_ain.sin_addr.s_addr = INADDR_ANY;
	s_ain.sin_port = htons(7777);	/* 7 is echo server */
	bind(sd, (struct sockaddr *) &s_ain, sizeof(s_ain));

	while (1) {
		size = sizeof(c_ain);
		recvfrom(sd, &byte, 1, 0, (struct sockaddr *) &c_ain, &size);
		sendto(sd, &byte, 1, 0, (struct sockaddr *) &c_ain, size);
		sendto(sd, &byte, 1, 0, (struct sockaddr *) &c_ain, size);
		sendto(sd, &byte, 1, 0, (struct sockaddr *) &c_ain, size);
		sendto(sd, &byte, 1, 0, (struct sockaddr *) &c_ain, size);
	}
	return 0;
}
