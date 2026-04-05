
// prelude.h

// sanity check our compiler/etc 
#include "predef.h"

// u16, the only type in thin c
typedef unsigned short u16;

// io syscalls
u16 in(void);
void out(u16 arg_ch);
