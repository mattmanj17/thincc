
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include "prelude.h"

static_assert(
	CHAR_BIT == 8,
	"bytes must be 8 bits");
static_assert(
	sizeof(u16) == 2,
	"u16 must be two bytes"
);

u16 thinc_getc(void) {
	int c = fgetc(stdin);
	if (c == EOF) {
		if (ferror(stdin)) {
			perror("fgetc");
			for (;;) {
				// do nothing
			}
		}
		else {
			return 0xFFFF;
		}
	}
	else {
		return (unsigned char)c;
	}
}

void thinc_putc(u16 arg_ch) {
	if (fputc((unsigned char)arg_ch, stdout) == EOF) {
		perror("fputc");
		for (;;) {
			// do nothing
		}
	}
}
