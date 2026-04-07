
// thincc.c

#include "prelude.h"
u16 mem[65536];
enum {
	M_RC_ABORT = 0xFFFE,
	M_RC_EXIT = 0xFFFF,
	M_EOF = 0xFFFF,
};
u16 step(u16 fn);
int main(void) {
	u16 v_fn = 0;
	do {
		v_fn = step(v_fn);
	} while (v_fn != M_RC_EXIT && v_fn != M_RC_ABORT);

	enum {
		DEBUG_SPIN = 1
	};
	volatile int v_debug_spin = DEBUG_SPIN;
	if (v_debug_spin) {
	l_debug_spin:
		goto l_debug_spin;
	}

	// return code from sentinel
	// M_RC_EXIT = no error
	// M_RC_ABORT = error
	return (v_fn != M_RC_EXIT);
}

// global argument registers

u16 step(u16 fn) {
	switch (fn) {
		
		enum {
			d_0 = 0x0000,
			d_1 = 0x0001,

			d_c_ch_file_size_max = 0x8000,
			d_ptr_last = 0xFFFF,

			// d_ptr_last - d_c_ch_file_size_max + 1
			d_ch_ptr_file_buffer_start = 0x8000
		};

		enum {
			g_arg_0 = d_0,
			g_arg_1 = d_1,
		};

		enum {
			fn_start,
			fn_fill_file_buffer,
			fn_print_file_buffer,
			fn_print_span,
		};

		

		// void start();
		case fn_start:
		{
			// fill_file_buffer(0);
			mem[g_arg_0] = d_0;
			return fn_fill_file_buffer;
		}
		break;

		// void fill_file_buffer(u16 c_ch_file_size_cur);
		case fn_fill_file_buffer:
		{
			u16 v_c_ch_file_size_cur = mem[g_arg_0];
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
			mem[g_arg_0] = v_c_ch_file_size_cur;
			return fn_fill_file_buffer;
		} 
		break;

		// void print_file_buffer(u16 c_ch_print);
		case fn_print_file_buffer:
		{

			// exit if not supposed to print any ch's
			if (mem[g_arg_0] == d_0) {
				return M_RC_EXIT;
			}

			// turn v_file_size into ch_ptr_last
			// NOTE we must dec by 1 to get the "last" ptr
			mem[g_arg_0] -= d_1;
			mem[g_arg_0] += d_ch_ptr_file_buffer_start;

			// move to ch_ptr_last
			mem[g_arg_1] = mem[g_arg_0];

			// ch_ptr_first = file_buffer_start
			mem[g_arg_0] = d_ch_ptr_file_buffer_start;

			// call print_span
			return fn_print_span;
		} 
		break;

		// void print_span(u16 ch_ptr_first, u16 ch_ptr_last);
		case fn_print_span:
		{
			u16 v_ch_ptr_first = mem[g_arg_0];
			u16 v_ch_ptr_last = mem[g_arg_1];

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
			mem[g_arg_0] = v_ch_ptr_first;
			return fn_print_span;
		} 
		break;

		// unknown function?
		default:
		{
			return M_RC_ABORT;
		} 
		break;
	}
}
