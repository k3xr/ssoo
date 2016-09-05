/* Abrir.c */
/*
 * Copyright 2005 datsi.fi.upm.es.
 * Universidad Politécnica de Madrid, Facultad de Informática
 *
 * Autor: Francisco Rosales
 * Versión: 1.0		Fecha: 22 Nov 2005
 * Descripción:
 *	Intenta abrir el fichero indicado con el modo de apertura
 *	y los permisos de creación indicados.
 *	Lo consiga o no, intenta mostrar la información del nodo-i
 *	del fichero.
 * Uso:
 *	Abrir archivo [modo [permisos]]
 */
#define MYNAME	"Abrir"

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static char * mode2str(mode_t m)
{
	static char str[20];
	str[0] = S_ISREG(m)?'-': S_ISDIR(m)?'d': S_ISLNK(m)?'l':
		 S_ISCHR(m)?'c': S_ISBLK(m)?'b': S_ISFIFO(m)?'p':
		 S_ISSOCK(m)?'s': '?'; 

	str[1] = (m & S_IRUSR)?'r':'-';
	str[2] = (m & S_IWUSR)?'w':'-';
	      if (m & S_ISUID)
	str[3] = (m & S_IXUSR)?'s':'S';
	      else
	str[3] = (m & S_IXUSR)?'x':'-';

	str[4] = (m & S_IRGRP)?'r':'-';
	str[5] = (m & S_IWGRP)?'w':'-';
	      if (m & S_ISGID)
	str[6] = (m & S_IXGRP)?'s':'S';
	      else
	str[6] = (m & S_IXGRP)?'x':'-';

	str[7] = (m & S_IROTH)?'r':'-';
	str[8] = (m & S_IWOTH)?'w':'-';
	      if (m & S_ISVTX)
	str[9] = (m & S_IXOTH)?'t':'T';
	      else
	str[9] = (m & S_IXOTH)?'x':'-';

	str[10] = '\0';
	return str;
}

int main(int argc, char * argv[])
{
	char * ptr = 0;
	int perm = 0777;
	int mode = 0;
	char * file = 0;
	int fd, ret;
	struct stat stt[1];

	switch (argc) {
	case 4:	/* perm */
		perm = strtol(argv[3], &ptr, 8);
		if (!ptr || *ptr) break;
	case 3:	/* mode */
		mode = strtol(argv[2], &ptr, 8);
		if (!ptr || *ptr) break;
	case 2:	/* file */
		file = argv[1];
	}
	if (!file) {
		printf("Uso: "MYNAME" archivo [modo [permisos]]\n");
		printf("   modo y permisos son números en octal.\n");
		printf("   modo es combinación de:\n");
		printf("	%-8s %06o\n", "O_RDONLY", O_RDONLY);
		printf("	%-8s %06o\n", "O_WRONLY", O_WRONLY);
		printf("	%-8s %06o\n", "O_RDWR", O_RDWR);
		printf("	%-8s %06o\n", "O_CREAT", O_CREAT);
		printf("	%-8s %06o\n", "O_EXCL", O_EXCL);
		printf("	%-8s %06o\n", "O_NOCTTY", O_NOCTTY);
		printf("	%-8s %06o\n", "O_TRUNC", O_TRUNC);
		printf("	%-8s %06o\n", "O_APPEND", O_APPEND);
		printf("	%-8s %06o\n", "O_NDELAY", O_NDELAY);
		printf("	%-8s %06o\n", "O_FSYNC", O_FSYNC);
		printf("	%-8s %06o\n", "O_ASYNC", O_ASYNC);
	} else {
		printf(MYNAME": open(\"%s\",0%o,0%o);\n", file, mode, perm);
		fd = open(file, mode, perm);
		if (fd < 0) {
			perror(MYNAME": open");
			ret = stat(argv[1], stt);
		} else {
			ret = fstat(fd, stt);
		}
		if (ret < 0) {
			perror(MYNAME": f/stat()");
		} else {
			printf(MYNAME":   dev   = %d\n", (int)stt->st_dev);
			printf(MYNAME":   ino   = %ld\n", stt->st_ino);
			printf(MYNAME":   mode  = %0o (%s)\n", stt->st_mode, mode2str(stt->st_mode));
			printf(MYNAME":   nlnks = %d\n", stt->st_nlink);
			printf(MYNAME":   uid   = %d (%s)\n", stt->st_uid, getpwuid(stt->st_uid)->pw_name);
			printf(MYNAME":   gid   = %d (%s)\n", stt->st_gid, getgrgid(stt->st_gid)->gr_name);
			printf(MYNAME":   rdev  = %d\n", (int)stt->st_rdev);
			printf(MYNAME":   size  = %ld\n", stt->st_size);
			printf(MYNAME":   blksz = %ld\n", stt->st_blksize);
			printf(MYNAME":   nblks = %d\n", (int)stt->st_blocks);
			printf(MYNAME":   atime = %s", ctime(&stt->st_atime));
			printf(MYNAME":   mtime = %s", ctime(&stt->st_mtime));
			printf(MYNAME":   ctime = %s", ctime(&stt->st_ctime));
		}
	}
	return 0;
}
