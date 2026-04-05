
// thincc.c

#include "thincc.h"

enum fn {
	fn_read_file,
	fn_write_file
};

DEF(d_0, 0x0000);
DEF(d_1, 0x0001);

DEF(d_rc_exit, M_RC_EXIT);
DEF(d_rc_abort, M_RC_ABORT);

DEF(d_mem_most, 0xFFFF);
DEF(d_eof, 0xFFFF);
DEF(d_file_size_most, 0x8000);

// size of file read in
u16 g_file_size = d_0;

u16 step(u16 fn) {
BRANCH(fn);
	CASE(fn_read_file);
		u16 v_x = 0x0000;
		u16 v_y = 0x0000;
		
		v_x = in();
		if (v_x == d_eof)
			return d_rc_exit;

		if (g_file_size >= d_file_size_most)
			return d_rc_abort;

		v_y = d_mem_most;
		v_y -= g_file_size;
		mem[v_y] = v_x;
	
		g_file_size += d_1;
		return fn_read_file;
	END;
	DEFAULT;
		return d_rc_abort;
	END;
CUT;
}
