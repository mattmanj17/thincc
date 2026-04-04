// thincc.h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef uint16_t u16;

u16 mem[UINT16_MAX+1];

u16 in()
{
	int ch = fgetc(stdin);
	if (ch == EOF)
	{
		if (ferror(stdin))
		{
			perror("fgetc");
		_halt_in:
			goto _halt_in;
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
	_halt_out:
		goto _halt_out;
	}
}
