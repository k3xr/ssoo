/* CPUs_prc.c */
/*
 * Copyright 2006 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 01 May 2006
 * Descripción:
 *	Se lanzan los hijos indicados como primer argumento.
 *	Se esperan los segundos indicados como segundo argumento,
 *	Los hijos son de CPU intensiva y van incrementando una variable propia.
 *	Vencido el tiempo de espera se paran los hijos y
 *	se calcula y muestra la cantidad total de cómputo realizada.
 * Uso:
 * 	CPUs_prc [nprocs:1..10 [nscecs:1..60]]
 */
#define MYNAME	"CPUs_prc"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void shared_free(void * ptr, unsigned size)
{
	munmap(ptr, size);
}

void * shared_alloc(unsigned size)
{
	void * ptr = NULL;
	int fd = open("/dev/zero", O_RDWR);

	if (fd < 0) return NULL;
	ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	return ptr;
}

void CPUa100(long long * ptr)
{
	*ptr = 0;

//	sched_yield();

	while(1) {
		(*ptr)++;
	}
}

int main(int argc, char * argv[])
{
	int i, n = 1, s = 10;
	pid_t * pids;
	long long ** work, total = 0;

	switch(argc) {
	case 3:
		s = atoi(argv[2]);
		if (s >= 60) s = 60;
		if (s <= 01) s = 01;
	case 2:
		n = atoi(argv[1]);
		if (n >= 10) n = 10;
		if (n <= 01) n = 01;
	case 1:
		break;

	default:
		printf("Uso: "MYNAME" [nprocs:1..10 [nsecs:1..60]]\n");
		return 1;
	}

	pids = calloc(sizeof(pid_t), n);
	work = calloc(sizeof(long long *), n);

	for (i = 0; i < n; i++) {
		work[i] = shared_alloc(sizeof(long long));

		switch(pids[i] = fork()) {
		case  0:CPUa100(work[i]);
			exit(0);
		case -1:
			perror(MYNAME": fork()");
			exit(1);
		}
	}

	usleep(s*1000*1000);

	for (i = 0; i < n; i++)
		kill(pids[i], SIGKILL);
	for (i = 0; i < n; i++)
		waitpid(pids[i], 0, 0);
	for (i = 0; i < n; i++)
		total += *(work[i]);
	printf("Total work done by %d process in %d seconds is = %lld\n", n, s, total);
	fflush(stdout);

	for (i = 0; i < n; i++)
		shared_free((void*)work[i], sizeof(long long));

	return 0;
}
