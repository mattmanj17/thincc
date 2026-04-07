
#include "prelude.h"
#include "thinc.defs"
THINC_BEGIN
enum cmd { start, read, print };
enum def { buf = 256 };
enum arg { min, max, cursor, ch };
case start: {
	thinc_store(min, buf);
	thinc_store(max, thinc_ptr_max);
	thinc_store(cursor, thinc_load(min));
	return read;
}
case read: {
	thinc_store(ch, thinc_getc());
	if (thinc_load(ch) == thinc_eof) 
		return print;
	if (thinc_load(cursor) < thinc_load(min)) 
		return thinc_exit_failure;
	if (thinc_load(cursor) >= thinc_load(max)) 
		return thinc_exit_failure;
	thinc_store(thinc_load(cursor), thinc_load(ch));
	thinc_add(cursor, 1);
	return read;
}
case print: {
	if (thinc_load(min) >= thinc_load(max))
		return thinc_exit_success;
	thinc_putc(thinc_load_indirect(min));
	thinc_add(min, 1);
	return print;
}
default: {
	return thinc_exit_failure;
}
THINC_END
