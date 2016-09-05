/*
 * pidedatos.c
 */

#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int num;

	printf("Dame un numero: ");
	sleep(5);
	scanf("%d", &num);
	printf("Me diste el %d.\n", num);
	fprintf(stderr, "Me diste el %d.\n", num);

	printf("Dame otro numero: ");
	scanf("%d", &num);
	printf("Ahora me diste el %d.\n", num);
	fprintf(stderr, "Ahora me diste el %d.\n", num);

	return 0;
}
