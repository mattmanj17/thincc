

/* usable '##' operator */
#define M_JOIN_(a, b)\
    a##b

#define M_JOIN(a, b)\
    M_JOIN_(a, b)
/**/

/* M_NAME = name_0_1_2_3, name_4_5_6_7, ... */
#define M_N\
    __COUNTER__

#define M_N_\
    M_JOIN(M_N, _)

#define M_N_2\
    M_JOIN(M_N_, M_N)

#define M_N_2_\
    M_JOIN(M_N_2, _)

#define M_N_3\
    M_JOIN(M_N_2_, M_N)

#define M_N_3_\
    M_JOIN(M_N_3, _)

#define M_N_4\
    M_JOIN(M_N_3_, M_N)

#define M_NAME\
    M_JOIN(name_, M_N_4)
/**/

/* warning control */
#ifdef _MSC_VER
    #define M_PUSH_IGNORE_TRUNCATING_CONSTANT_CAST_WARNING \
        __pragma(warning(push))\
        __pragma(warning(disable:4310))

    #define M_RESET_WARNINGS\
        __pragma(warning(pop))
#else
    #error unsupported compiler
#endif
/**/

/* static assertions */
#define M_ASSERT(x)\
    typedef char M_NAME[(x) ? 1 : -1]

#define M_ASSERT_CAST_EQ(type, lhs, rhs)\
    M_PUSH_IGNORE_TRUNCATING_CONSTANT_CAST_WARNING\
    M_ASSERT(((type)(lhs)) == (rhs))\
    M_RESET_WARNINGS
/**/

/* typedef helpers */
#define M_U(n)\
    M_JOIN(u, n)
#define M_U_MAX(n)\
    M_JOIN(\
        M_JOIN(U, n),\
        _MAX\
    )
#define M_TYPEDEF_U(n, type, max)\
    typedef type M_U(n);\
    enum { M_U_MAX(n) = (max) };\
    M_ASSERT(sizeof(M_U(n)) == ((n) / 8));\
    M_ASSERT_CAST_EQ(\
        M_U(n),\
        M_U_MAX(n),\
        M_U_MAX(n)\
    );\
    M_ASSERT_CAST_EQ(\
        M_U(n),\
        -1,\
        M_U_MAX(n)\
    );\
    M_ASSERT_CAST_EQ(\
        M_U(n),\
        1 + M_U_MAX(n),\
        0\
    )
/**/

/* typedefs */
M_TYPEDEF_U(8, unsigned char, 0xFF);
M_TYPEDEF_U(16, unsigned short, 0xFFFF);
/**/

enum {
    ABORT   = 0xFFFE,
    EXIT    = 0xFFFF,
    NIL     = 0xFFFF,
};
u16         get(u16 ptr);
u16         deref(u16 ptr_ptr);
void        set(u16 ptr, u16 val);
void        sub(u16 ptr, u16 val);
void        add(u16 ptr, u16 val);
u16         in(void);
void        out(u16 ch);

u16         main_thinc(u16);

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
