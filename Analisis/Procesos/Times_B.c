/* Times_B.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Polit�cnica de Madrid, Facultad de Inform�tica
 *
 * Autor: Francisco Rosales
 * Versi�n: 1.0		Fecha: 21 Sep 2004
 * Descripci�n:
 *	Calcula los tiempos consumidos por la ejecuci�n del
 *	mandato indicado.
 *	Utiliza las funciones auxiliares fork_command y wait_process.
 *	El padre ignora las se�ales generables por teclado.
 * Uso:
 *	Times_B mandato [argumentos...]
 */
#define MYNAME	"Times_B"

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Lanza el mandato indicado.
// Retorna: -1 (si error) o PID del proceso hijo.
int fork_command(char * argv[])
{
	int pid = fork();

	if (pid != 0)
		return pid;
	execvp(argv[0], argv);
	perror(MYNAME);
	exit(1);
}

// Espera la terminaci�n de un proceso.
// Retorna: -1 (si error) o estado de terminaci�n.
int wait_process(int pid)
{
	int status;

	pid = waitpid(pid, &status, 0);
	return (pid == -1) ? -1 : status;
}



/* 
 * Cronometra la ejecuci�n de un mandato.
 */
int chron_command(char * argv[]);

int main(int argc, char * argv[])
{
	int ret;

	ret = chron_command( NULL );
	if (ret == -1) {
		perror(MYNAME);
		return 1;
	}
	ret = chron_command(argv+1);
	if (ret == -1) {
		perror(MYNAME);
		return 1;
	}
	return 0;
}





#include <signal.h>
#include <time.h>
#include <sys/times.h>

/*
 * Conversiones de ticks a segundos y milisegundos.
 */
#define tck2sec(tck) (int)((tck) / CLK_TCK)
#define tck2msec(tck) (int)(((tck) % CLK_TCK) * 1000L / CLK_TCK)

/* 
 * Cronometra la ejecuci�n de un mandato.
 */
int chron_command(char * argv[])
{
	static clock_t startticks = 0;
	clock_t realticks, userticks, systticks;
	struct tms ticks;
	int pid, status;
#ifndef CLK_TCK
	int CLK_TCK = sysconf(_SC_CLK_TCK);
#endif

	if (argv == NULL) {
		startticks = times(NULL);
		return (int)startticks;
	}

	realticks = -startticks;
	userticks = 0;
	systticks = 0;

	if (argv[0] != NULL) {
		realticks = -times(&ticks);
		userticks = -ticks.tms_utime - ticks.tms_cutime;
		systticks = -ticks.tms_stime - ticks.tms_cstime;
		if ((pid = fork_command( argv )) == -1)
			return -1;
		signal(SIGINT,  SIG_IGN);   /* <<=== Ignorar se�ales de      */
		signal(SIGQUIT, SIG_IGN);   /* <<=== terminaci�n interactiva */
		if ((status = wait_process(pid)) == -1)
			return -1;
	}
		realticks += times(&ticks);
		userticks += ticks.tms_utime + ticks.tms_cutime;
		systticks += ticks.tms_stime + ticks.tms_cstime;
		fprintf(stderr, MYNAME": ��� Real:%d.%03d\" User:%d.%03d\" Syst:%d.%03d\"\n",
			tck2sec(realticks), tck2msec(realticks),
			tck2sec(userticks), tck2msec(userticks),
			tck2sec(systticks), tck2msec(systticks));
	return 0;
}
