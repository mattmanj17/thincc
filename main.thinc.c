
#include ".thinc.h"
enum cases { start, read, print };
enum defs { 
    buffer_ptr = 256, 
    buffer_end = 0xFFFF,
};
enum vars { begin, end, ch };
u16 main_thinc(u16 i) { 
    switch (i) {
    case start:
        set(begin, buffer_ptr);
        set(end, buffer_ptr);
        return read;
    case read:
        set(ch, in());
        if (get(ch) == NIL) 
            return print;
        if (get(end) >= buffer_end) 
            return ABORT;
        set(get(end), get(ch));
        add(end, 1);
        return read;
    case print:
        if (get(begin) >= get(end))
            return EXIT;
        out(deref(begin));
        add(begin, 1);
        return print;
    default: 
        return ABORT;
    }
}
