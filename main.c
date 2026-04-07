
#include "prelude.h"
int main(void) {
	thinc_u16 cmd = 0;
	do {
		thinc_u16 cmd_next = thinc_main(cmd);
		cmd = cmd_next;
	}
	while (
		cmd != thinc_exit_success &&
		cmd != thinc_exit_failure
	);
	for (;;) {
		// do nothing
	}
}
