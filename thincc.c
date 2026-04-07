
#include "prelude.h"
thinc_u16 mem[65536];


int main(void) {
	thinc_u16 instruction = 0;
	do {
		instruction = thinc_next(instruction);
	}
	while (
		instruction != thinc_exit_success &&
		instruction != thinc_exit_failure
	);
	for (;;) {
		// do nothing
	}
}

// global argument registers

thinc_u16 thinc_next(thinc_u16 fn) {
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
		thinc_u16 v_c_ch_file_size_cur = mem[g_arg_0];
		thinc_u16 v_ch = d_0;
		thinc_u16 v_ch_ptr = d_ch_ptr_file_buffer_start;

		// read
		v_ch = thinc_getc();
		if (v_ch == thinc_eof) {
			// NOTE v_file_size is sitting in g_arg_0
			return fn_print_file_buffer;
		}

		// buffer exhasted
		if (v_c_ch_file_size_cur >= d_c_ch_file_size_max) {
			return thinc_exit_failure;
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
			return thinc_exit_success;
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
		thinc_u16 v_ch_ptr_first = mem[g_arg_0];
		thinc_u16 v_ch_ptr_last = mem[g_arg_1];

		if (v_ch_ptr_first > v_ch_ptr_last) {
			// done printing
			return thinc_exit_success;
		}

		// print
		thinc_putc(mem[v_ch_ptr_first]);

		// handle case where v_ch_ptr_last is the end of mem
		if (v_ch_ptr_first == d_ptr_last) {
			return thinc_exit_success;
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
		return thinc_exit_failure;
	}
	break;
	}
}
