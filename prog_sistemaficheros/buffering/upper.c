/*
 * upper.c
 */

#include <ctype.h>
#include <stdio.h>

int main(void)
{
	int ch;

	while ((ch=getchar()) != EOF)
	{
		if (isprint(ch) || isspace(ch)) {
			ch = toupper(ch);
			putchar(ch);
		} else {
	fprintf(stderr,"upper: %c no parece un caracter normal\n", (char)ch);
		}
	}
	return 0;
}
