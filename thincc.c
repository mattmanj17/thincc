
// thincc.c

#include "thincc.h"

ENUM(e_rc_exit, 0xFFFF);
ENUM(e_rc_abort, 0xFFFE);

ENUM(e_fn_start, 0x0000);

ENUM(e_mem_most, 0xFFFF);
ENUM(e_eof, 0xFFFF);
ENUM(e_file_size_most, 0x8000);

u16 g_file_size = 0x0000;

u16 step(u16 arg_fn) {
BRANCH(arg_fn);
	CASE(e_fn_start);
		u16 v_x = 0x0000;
		u16 v_y = 0x0000;
		
		v_x = in();
		if (v_x == e_eof)
			return e_rc_exit;

		if (g_file_size >= e_file_size_most)
			return e_rc_abort;

		v_y = e_mem_most;
		v_y -= g_file_size;
		mem[v_y] = v_x;
	
		g_file_size += 0x0001;
		return e_fn_start;
	END;
	DEFAULT;
		return e_rc_abort;
	END;
CUT;
}
