
// thincc.h

#if defined _thincc_h_
#error thincc.h must not be included multiple times
#else
#define _thincc_h_
#endif

// includes
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

// helper macros
/*{*/
#define STATEMENT(x) do { x } while (0)

#define SPIN_EXIT 1
#if SPIN_EXIT
#define EXIT(x) STATEMENT((void)(x); while(1) {})
#else
#include <stdlib.h>
#define EXIT(x) exit(x)
#endif
/*}*/

// u16, the only type in thin c
typedef uint16_t u16;

// thin c "heap" memory is 64k 16 bit words.
// distinct from globals/locals in the including file.
u16 mem[UINT16_MAX+1];

// "syscalls"
/*{*/
// fgetc stdin
u16 in(void)
{
	int ch = fgetc(stdin);
	if (ch == EOF)
	{
		if (ferror(stdin))
		{
			perror("fgetc");
			EXIT(1);
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
// fputc stdout
void out(u16 ch)
{
	if (fputc((unsigned char)ch, stdout) == EOF) 
	{
		perror("fputc");
		EXIT(1);
	}
}
/*}*/

// main function. 
// calls step function in a loop,
// until it returns a sentinel.
u16 step(u16 fn);
int main(void)
{
	u16 fn = 0;
	do
	{
		fn = step(fn);
	} while (fn != 0xFFFF);

	EXIT(0);
}

// helper for the macros below
#define REQ_SEMI STATEMENT(;)

// macros intended to be consumed from thin c code
/*{*/
#define ENUM(name, value)	enum { name = value }

#define BRANCH(fn)			switch (fn) { REQ_SEMI
#define CUT					} REQ_SEMI

#define CASE(fn)			case fn: { REQ_SEMI
#define DEFAULT				default: { REQ_SEMI
#define END					} break
/*}*/
