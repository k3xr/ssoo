/* TCP_s.c */
/*
 * Copyright 2003 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 9 May 2003
 * Descripción:
 *	Servidor de eco (en el puerto 7777), para Clientes remotos.
 *	Comunicación vía sockets TCP.
 * Uso:
 *	TCP_s [portnumber]
 */
#define MYNAME	"TCP_s"

#include <arpa/inet.h>			/* inet_* */
#include <netinet/in.h>			/* IP*, sockaddr_in, ntoh?, hton?, etc. */
#include <netinet/tcp.h>		/* TCPOPT_*, etc. */
#include <sys/socket.h>			/* socket */
#include <sys/types.h>			/* socket */
#include <sys/un.h>			/* sockaddr_un */
#include <sys/wait.h>			/* waitpid */
#include <netdb.h>			/* gethostbyname */

#include <ctype.h>
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

void SIGCHLD_Handler(int signo)
{
	signal(SIGCHLD, SIGCHLD_Handler);
	if (signo == SIGCHLD)
		waitpid(-1, NULL, WNOHANG);
}

int main(int argc, char * argv[])
{
	int sd, cd, size, ret;
	char ch;
	short int port;
	struct sockaddr_in s_ain, c_ain;

	Timeout_Control(0);
	SIGCHLD_Handler(0);

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd == -1)
		perror(argv[0]);

	if (argc > 1)
		port = atoi(argv[1]);
	else
		port = 7777;

	bzero((char *) &s_ain, sizeof(s_ain));
	s_ain.sin_family = AF_INET;
	s_ain.sin_addr.s_addr = INADDR_ANY;
	s_ain.sin_port = htons(port);	/* 7 == echo port */

	ret = bind(sd, (struct sockaddr *) &s_ain, sizeof(s_ain));
	if (ret == -1)
		perror(argv[0]);

	listen(sd, 5);

	while (1) {
		size = sizeof(c_ain);
		cd = accept(sd, (struct sockaddr *) &c_ain, &size);
		switch (fork()) {
		case -1:
			perror("echo server");
			return 1;
		case 0:
			close(sd);
			while (recv(cd, &ch, 1, 0) == 1) {
				ch = toupper(ch);
				send(cd, &ch, 1, 0);
			}
			close(cd);
			return 0;
		default:
			close(cd);
		} /* switch */
	} /* while */
} /* main */
