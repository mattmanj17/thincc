
// thincc.c

#include "thincc.h"

enum fn {
	fn_read_file,
	
	fn_write_file,
	fn_write_file_loop,
};

DEF(d_0, 0x0000);
DEF(d_1, 0x0001);

DEF(d_rc_exit, M_RC_EXIT);
DEF(d_rc_abort, M_RC_ABORT);

DEF(d_mem_most, 0xFFFF);
DEF(d_eof, 0xFFFF);
DEF(d_file_size_most, 0x8000);

// global argument registers
u16 g_arg_0 = d_0;
u16 g_arg_1 = d_0;
u16 g_arg_2 = d_0;

u16 step(u16 fn) {
BRANCH(fn);

	// fn_read_file
	CASE(fn_read_file);
		u16 v_file_size = g_arg_0;
		u16 v_x = 0x0000;
		u16 v_y = 0x0000;
		
		v_x = in();
		if (v_x == d_eof) {
			return fn_write_file;
		}

		if (v_file_size >= d_file_size_most) {
			return d_rc_abort;
		}

		v_y = d_mem_most;
		v_y -= v_file_size;
		mem[v_y] = v_x;
	
		v_file_size += d_1;
		g_arg_0 = v_file_size;
		return fn_read_file;
	END;

	// fn_write_file
	CASE(fn_write_file);
		g_arg_1 = d_0;
		g_arg_2 = d_mem_most;
		return fn_write_file_loop;
	END;
	CASE(fn_write_file_loop);
		u16 v_file_size = g_arg_0;	
		u16 v_i = g_arg_1;
		u16 v_j = g_arg_2;

		if (v_i == v_file_size){
			return d_rc_exit;
		}

		out(mem[v_j]);
		v_i += d_1;
		v_j -= d_1;
		g_arg_1 = v_i;
		g_arg_2 = v_j;
		return fn_write_file_loop;
	END;

	// unknown function?
	DEFAULT;
		return d_rc_abort;
	END;
CUT;
}
