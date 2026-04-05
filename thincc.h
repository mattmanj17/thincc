
// thincc.h

#include "prelude.h"

#if defined M_THINCC
	#error thincc.h defines 'main', do not include it repeatedly
#else
	#define M_THINCC
#endif

// thin c "heap" memory is 64k 16 bit words.
// distinct from globals/locals in the including file.
u16 mem[65536];

// main function. 
// call step() until it returns a sentinel.
#define M_FN_START	((u16)0x0000)
#define M_RC_EXIT	((u16)0xFFFF)
#define M_RC_ABORT	((u16)0xFFFE)
u16 step(u16 arg_fn);
int main(void)
{
	u16 v_fn = M_FN_START;
	do
	{
		v_fn = step(v_fn);
	} while (v_fn != M_RC_EXIT && v_fn != M_RC_ABORT);

	enum { DEBUG_SPIN = 1 };
	volatile int v_debug_spin = DEBUG_SPIN;
	if (v_debug_spin)
	{
	l_debug_spin:
		goto l_debug_spin;
	}

	// return code from sentinel
	// M_RC_EXIT = no error
	// M_RC_ABORT = error
	return (v_fn != M_RC_EXIT);
}

// macros intended to be consumed from thin c code
/*{*/
#define ENUM(name, value)	enum { name = value }

#define BRANCH(fn)			switch (fn) { do { ; } while (0)
#define CUT					} do { ; } while (0)

#define CASE(fn)			case fn: { do { ; } while (0)
#define DEFAULT				default: { do { ; } while (0)
#define END					} break
/*}*/
