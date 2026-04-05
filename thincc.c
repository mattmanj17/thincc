
// thincc.c

#include "thincc.h"

ENUM(_e_START, 0x0000);
ENUM(_e_EXIT, 0xFFFF);

u16 step(u16 fn) {
BRANCH(fn);
	CASE(_e_START);
		return _e_EXIT;
	END;
	DEFAULT;
		return _e_EXIT;
	END;
CUT;
}
