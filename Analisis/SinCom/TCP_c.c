/* TCP_c.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Cliente de un Servidor remoto que hace eco.
 *	Comunicación vía sockets TCP.
 * Uso:
 *	TCP_c [hostname [portnumber] ]
 */
#define MYNAME	"TCP_c"

#include <arpa/inet.h>			/* inet_* */
#include <netinet/in.h>			/* IP*, sockaddr_in, ntoh?, hton?, etc. */
#include <netinet/tcp.h>		/* TCPOPT_*, etc. */
#include <sys/socket.h>			/* socket */
#include <sys/types.h>			/* socket */
#include <sys/un.h>			/* sockaddr_un */
#include <netdb.h>			/* gethostbyname */

#include <sys/poll.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Timeout_Control(int signo)
{
	int timeout;
	char * text = "TIMEOUT!...Terminating now.\n";

	if (!signo) {
		text = getenv("TIMEOUT");
		if (text) {
			signal(SIGALRM, Timeout_Control);
			timeout = atoi(text);
			if (timeout >= 0)
				alarm(timeout);
		}
	} else {
		write(2, text, strlen(text));
		exit(0);
	}
}

int bidirectional_echo(int in1, int out1, int in2, int out2)
{
	struct pollfd ufds[4];
	int inputs = 2;
	unsigned char byte;

	ufds[0].fd = in1;	ufds[0].events = POLLIN;
	ufds[1].fd = out1;	ufds[1].events = 0;
	ufds[2].fd = in2;	ufds[2].events = POLLIN;
	ufds[3].fd = out2;	ufds[3].events = 0;

	while(poll(ufds,4,2000) >= 0 && inputs > 0)
	{
		if (ufds[2].revents)
			switch(read(ufds[2].fd, &byte, 1)) {
			case 1:
				if (write(ufds[3].fd, &byte, 1) < 0)
					return -2;
				break;
			case 0:
				ufds[2].fd = -1;
				inputs--;
				break;
			case -1:
				return -3;
			}

		if (ufds[0].revents)
			switch(read(ufds[0].fd, &byte, 1)) {
			case 1:
				if (write(ufds[1].fd, &byte, 1) < 0)
					return -4;
				break;
			case 0:	shutdown(ufds[1].fd, SHUT_WR);
				ufds[0].fd = -1;
				inputs--;
				break;
			case -1:
				return -5;
			}
	}
	if (!inputs)
		return 0;
	return -1;
}

int main(int argc, char *argv[])
{
	int cd, ret;
	char * hostname;
	short int port;
	struct sockaddr_in s_sain;
	struct hostent *hp;

	Timeout_Control(0);

	if (argc > 1)
		hostname = argv[1];
	else
		hostname = "localhost";

	hp = gethostbyname(argv[1]);
	if (!hp) {
		perror(hostname);
		return 1;
	}

	if (argc > 2)
		port = atoi(argv[2]);
	else
		port = 7777;

	bzero((char *) &s_sain, sizeof(s_sain));
	s_sain.sin_family = AF_INET;
	memcpy(&(s_sain.sin_addr), hp->h_addr, hp->h_length);
	s_sain.sin_port = htons(port);	/* 7 == echo port */

	cd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (cd == -1) {
		perror(argv[0]);
		return 1;
	}

	ret = connect(cd, (struct sockaddr *) &s_sain, sizeof(s_sain));
	if (ret == -1) {
		perror(argv[0]);
		return 1;
	}

#if 0
	char ch;
	while((ret = read(0, &ch, 1)) == 1) {
		write(cd, &ch, 1);
		read(cd, &ch, 1);
		write(1, &ch, 1);
	}
#else
	ret = bidirectional_echo(0,cd,cd,1);
#endif
	if (ret < 0) {
		perror(argv[0]);
		fprintf(stderr, "%s: %d\n", argv[0], ret);
		return 1;
	}

	return 0;
}
