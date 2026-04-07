
#include "prelude.h"
#include "thinc.defs"
THINC_BEGIN
enum cases { start, read, print };
enum defs { 
    buffer_ptr = 256, 
    buffer_end = 0xFFFF,
};
enum vars { begin, end, ch };
case start:
    set(begin, buffer_ptr);
    set(end, buffer_ptr);
    return read;
case read:
    set(ch, in());
    if (get(ch) == eof) 
        return print;
    if (get(end) >= buffer_end) 
        return rc_abort;
    set(get(end), get(ch));
    inc(end);
    return read;
case print:
    if (get(begin) >= get(end))
        return rc_exit;
    out(deref(begin));
    inc(begin);
    return print;
default: 
    return rc_abort;
THINC_END
