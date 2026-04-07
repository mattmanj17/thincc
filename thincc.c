
// thincc.c

#include "prelude.h"

// thin c "heap" memory is 64k 16 bit words.
// distinct from globals/locals in the including file.
u16 mem[65536];

// main function. 
// call step() until it returns a sentinel.
#define M_RC_EXIT	((u16)0xFFFF)
#define M_RC_ABORT	((u16)0xFFFE)
#define M_EOF		((u16)0xFFFF)
u16 step(u16 fn);
int main(void)
{
	u16 v_fn = 0;
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
#define DEF(name, value)	enum { name = value }

#define BRANCH(fn)			switch (fn) { do { ; } while (0)
#define CUT					} do { ; } while (0)

#define CASE(fn)			case fn: { do { ; } while (0)
#define DEFAULT				default: { do { ; } while (0)
#define END					} break
/*}*/

enum fn {
	fn_start,
	fn_fill_file_buffer,
	fn_print_file_buffer,
	fn_print_span,
};

DEF(d_0, 0x0000);
DEF(d_1, 0x0001);

DEF(d_c_ch_file_size_max,		0x8000);
DEF(d_ptr_last,					0xFFFF);

// d_ptr_last - d_c_ch_file_size_max + 1
DEF(d_ch_ptr_file_buffer_start, 0x8000);

// global argument registers
u16 g_arg_0 = d_0;
u16 g_arg_1 = d_0;

u16 step(u16 fn) {
BRANCH(fn);

	// void start();
	CASE(fn_start);
		// fill_file_buffer(0);
		g_arg_0 = d_0;
		return fn_fill_file_buffer;
	END;

	// void fill_file_buffer(u16 c_ch_file_size_cur);
	CASE(fn_fill_file_buffer);
		u16 v_c_ch_file_size_cur = g_arg_0;
		u16 v_ch = d_0;
		u16 v_ch_ptr = d_ch_ptr_file_buffer_start;
		
		// read
		v_ch = in();
		if (v_ch == M_EOF) {
			// NOTE v_file_size is sitting in g_arg_0
			return fn_print_file_buffer;
		}

		// buffer exhasted
		if (v_c_ch_file_size_cur >= d_c_ch_file_size_max) {
			return M_RC_ABORT;
		}

		// store
		v_ch_ptr += v_c_ch_file_size_cur;
		mem[v_ch_ptr] = v_ch;
		v_c_ch_file_size_cur += d_1;

		// recurse
		g_arg_0 = v_c_ch_file_size_cur;
		return fn_fill_file_buffer;
	END;

	// void print_file_buffer(u16 c_ch_print);
	CASE(fn_print_file_buffer);
		
		// exit if not supposed to print any ch's
		if (g_arg_0 == d_0) {
			return M_RC_EXIT;
		}

		// turn v_file_size into ch_ptr_last
		// NOTE we must dec by 1 to get the "last" ptr
		g_arg_0 -= d_1;
		g_arg_0 += d_ch_ptr_file_buffer_start;
		
		// move to ch_ptr_last
		g_arg_1 = g_arg_0;
		
		// ch_ptr_first = file_buffer_start
		g_arg_0 = d_ch_ptr_file_buffer_start;
		
		// call print_span
		return fn_print_span;
	END;

	// void print_span(u16 ch_ptr_first, u16 ch_ptr_last);
	CASE(fn_print_span);
		u16 v_ch_ptr_first = g_arg_0;	
		u16 v_ch_ptr_last = g_arg_1;

		if (v_ch_ptr_first > v_ch_ptr_last) {
			// done printing
			return M_RC_EXIT;
		}

		// print
		out(mem[v_ch_ptr_first]);
		
		// handle case where v_ch_ptr_last is the end of mem
		if (v_ch_ptr_first == d_ptr_last) {
			return M_RC_EXIT;
		}

		// inc
		v_ch_ptr_first += d_1;

		// recurse
		g_arg_0 = v_ch_ptr_first;
		return fn_print_span;
	END;

	// unknown function?
	DEFAULT;
		return M_RC_ABORT;
	END;
CUT;
}
