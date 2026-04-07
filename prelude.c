
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "prelude.h"

static_assert(
	CHAR_BIT == 8,
	"bytes must be 8 bits");
static_assert(
	sizeof(u16) == 2,
	"u16 must be two bytes"
	);

u16 in(void)
{
	int v_ch = fgetc(stdin);
	if (v_ch == EOF)
	{
		if (ferror(stdin))
		{
			perror("fgetc");
			for (;;) {}
		}
		else
		{
			return 0xFFFF;
		}
	}
	else
	{
		return (u16)v_ch;
	}
}

void out(u16 arg_ch)
{
	if (fputc((unsigned char)arg_ch, stdout) == EOF)
	{
		perror("fputc");
		for (;;) {}
	}
}
