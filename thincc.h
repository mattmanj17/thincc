#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint16_t u16;
void out(u16 ch)
{
	if (fputc(ch, stdout) == EOF) 
	{
		perror("fputc");
		exit(EXIT_FAILURE);
	}
}
