
#include "prelude.h"
int main(void) {
	thinc_u16 current_instruction = 0;
	do {
		thinc_u16 next_instruction = thinc_run(current_instruction);
		current_instruction = next_instruction;
	}
	while (
		current_instruction != thinc_exit_success &&
		current_instruction != thinc_exit_failure
	);
	for (;;) {
		// do nothing
	}
}