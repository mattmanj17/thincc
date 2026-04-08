
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include ".thinc.h"

u16  mem[USHRT_MAX + 1];
u16  get(u16 ptr)          { return mem[ptr];          }
u16  deref(u16 ptr_ptr)    { return mem[mem[ptr_ptr]]; }
void set(u16 ptr, u16 val) { mem[ptr]  = val;          }
void sub(u16 ptr, u16 val) { mem[ptr] -= val;          }
void add(u16 ptr, u16 val) { mem[ptr] += val;          }

u16 in(void) {
    int c = fgetc(stdin);
    if (c != EOF)            
        return (u16)c;
    if (feof(stdin))        
        return NIL;
    perror("fgetc");
    for (;;) {}
}
void out(u16 val) {
    u8 ch = (u8)val;
    if (fputc(ch, stdout) != EOF) 
        return;
    perror("fputc");
    for (;;) {}
}

int main(void) {
    u16 i = 0;
    while (i != EXIT && i != ABORT) {
        i = main_thinc(i);
    }
    for (;;) {}
}
