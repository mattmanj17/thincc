
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "prelude.h"
static_assert(CHAR_BIT == 8, "!!!");
static_assert(sizeof(u8) == 1, "!!!");
static_assert(sizeof(u16) == 2, "!!!");
static_assert(UCHAR_MAX == 0xFF, "!!!");
static_assert(USHRT_MAX == 0xFFFF, "!!!");

u16  mem[USHRT_MAX + 1];
u16  get(u16 ptr)          { return mem[ptr];          }
u16  deref(u16 ptr_ptr)    { return mem[mem[ptr_ptr]]; }
void set(u16 ptr, u16 val) { mem[ptr]  = val;          }
void sub(u16 ptr, u16 val) { mem[ptr] -= val;          }
void dec(u16 ptr)          { sub(ptr, 1);              }
void add(u16 ptr, u16 val) { mem[ptr] += val;          }
void inc(u16 ptr)          { add(ptr, 1);              }

u16 in(void) {
    int c = fgetc(stdin);
    if (c != EOF)            
        return (u16)c;
    if (feof(stdin))        
        return eof;
    perror("fgetc");
    exit(1);
}
void out(u16 val) {
    u8 ch = (u8)val;
    if (fputc(ch, stdout) != EOF) 
        return;
    perror("fputc");
    exit(1);
}
