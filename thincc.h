#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint16_t u16;
u16 in()
{
	int ch = fgetc(stdin);
	if (ch == EOF)
	{
		if (ferror(stdin))
		{
			perror("fgetc");
			exit(EXIT_FAILURE);
		}
		else
		{
			return 0xFFFF;
		}
	}
	else
	{
		return (u16)ch;
	}
}
void out(u16 ch)
{
	if (fputc(ch, stdout) == EOF) 
	{
		perror("fputc");
		exit(EXIT_FAILURE);
	}
}
