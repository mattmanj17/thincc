#include ".thinc.h"

#ifdef _MSC_VER
    // XXX : harden this against different CRT versions
    __declspec(dllimport) void* __cdecl __acrt_iob_func(unsigned _Ix);
    #define M_STDIN\
        (__acrt_iob_func(0))
    #define M_STDOUT\
        (__acrt_iob_func(1))
    #define M_EOF\
        (-1)
#else
    #error unsupported compiler
#endif

extern int fgetc(void* stream);
extern int fputc(int ch, void* stream);
extern int feof(void* stream);
extern void perror(const char *s);

u16  mem[U16_MAX + 1];
u16  get(u16 ptr)          { return mem[ptr];          }
u16  deref(u16 ptr_ptr)    { return mem[mem[ptr_ptr]]; }
void set(u16 ptr, u16 val) { mem[ptr]  = val;          }
void sub(u16 ptr, u16 val) { mem[ptr] -= val;          }
void add(u16 ptr, u16 val) { mem[ptr] += val;          }

u16 in(void) {
    int c = fgetc(M_STDIN);
    if (c != M_EOF)            
        return (u16)c;
    if (feof(M_STDIN))        
        return NIL;
    perror("fgetc");
    for (;;) {}
}
void out(u16 val) {
    u8 ch = (u8)val;
    if (fputc(ch, M_STDOUT) != M_EOF) 
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
