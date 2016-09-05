/* UDP_c.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Cliente de un Servidor remoto que hace eco.
 *	Comunicación vía sockets UDP.
 * Uso:
 *	UDP_c hostname
 */
#define MYNAME	"UDP_c"

#include <arpa/inet.h>			/* inet_* */
#include <netinet/in.h>			/* IP*, sockaddr_in, ntoh?, hton?, etc. */
#include <netinet/tcp.h>		/* TCPOPT_*, etc. */
#include <sys/socket.h>			/* socket */
#include <sys/types.h>			/* socket */
#include <sys/un.h>			/* sockaddr_un */
#include <netdb.h>			/* gethostbyname */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int cd;
	struct sockaddr_in s_sain;
	struct hostent *hp;
	char ch;

	cd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	hp = gethostbyname(argv[1]);
	bzero((char *) &s_sain, sizeof(s_sain));
	s_sain.sin_family = AF_INET;
	memcpy(&(s_sain.sin_addr), hp->h_addr, hp->h_length);
	s_sain.sin_port = htons(7777);	/* 7 is echo port */

	connect(cd, (struct sockaddr *) &s_sain, sizeof(s_sain));

	while (read(0, &ch, 1) == 1) {
		send(cd, &ch, 1, 0);
		recv(cd, &cd, 1, 0); write(1, &ch, 1);
		recv(cd, &cd, 1, 0); write(1, &ch, 1);
		recv(cd, &cd, 1, 0); write(1, &ch, 1);
		recv(cd, &cd, 1, 0); write(1, &ch, 1);
	}
	return 0;
}
