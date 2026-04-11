
// macro gunk

// usable "##"
#define JOIN_R(a,b)             a##b
#define JOIN_2(a,b)             JOIN_R(a,b)
#define JOIN_3(a,b,c)           JOIN_2(JOIN_2(a,b),c)
#define JOIN_4(a,b,c,d)         JOIN_2(JOIN_3(a,b,c),d)
#define JOIN_5(a,b,c,d,e)       JOIN_2(JOIN_4(a,b,c,d),e)
#define JOIN_6(a,b,c,d,e,f)     JOIN_2(JOIN_5(a,b,c,d,e),f)
#define JOIN_7(a,b,c,d,e,f,g)   JOIN_2(JOIN_6(a,b,c,d,e,f),g)
#define JOIN_8(a,b,c,d,e,f,g,h) JOIN_2(JOIN_7(a,b,c,d,e,f,g),h)

// MAKE_NAME : name_0_1_2_3, name_4_5_6_7, etc
#define COUNTER __COUNTER__
#define MAKE_NAME JOIN_8(name_,COUNTER,_,COUNTER,_,COUNTER,_,COUNTER)

// "typedef char foo[-1]" is an error, so this magic
// acts as a portable static assert
#define STATIC_ASSERT(x) typedef char MAKE_NAME[(x) ? 1 : -1]

// warning gunk
#ifdef _MSC_VER
    #define MSVC_PRAGMA_WARNING_PUSH        __pragma(warning(push))
    #define MSVC_PRAGMA_WARNING_DISABLE(x)  __pragma(warning(disable:x))
    #define MSVC_PRAGMA_WARNING_POP         __pragma(warning(pop))
#else
    #define MSVC_PRAGMA_WARNING_PUSH
    #define MSVC_PRAGMA_WARNING_DISABLE(x)
    #define MSVC_PRAGMA_WARNING_POP
#endif



// fixed width unsigned types

typedef unsigned char u8;
#define U8_MAX 0xFFu
STATIC_ASSERT(sizeof(u8) == 1);
STATIC_ASSERT((u8)U8_MAX == U8_MAX);
STATIC_ASSERT((u8)-1 == U8_MAX);
MSVC_PRAGMA_WARNING_PUSH
    ///* cast truncates constant value * /
    MSVC_PRAGMA_WARNING_DISABLE(4310)
    STATIC_ASSERT((u8)(1 + U8_MAX) == 0);
MSVC_PRAGMA_WARNING_POP

typedef unsigned short u16;
#define U16_MAX 0xFFFFu
STATIC_ASSERT(sizeof(u16) == 2);
STATIC_ASSERT((u16)U16_MAX == U16_MAX);
STATIC_ASSERT((u16)-1 == U16_MAX);
MSVC_PRAGMA_WARNING_PUSH
    MSVC_PRAGMA_WARNING_DISABLE(4310)
    STATIC_ASSERT((u16)(1 + U16_MAX) == 0);
MSVC_PRAGMA_WARNING_POP

typedef unsigned int u32;
#define U32_MAX 0xFFFFFFFFul
STATIC_ASSERT(sizeof(u32) == 4);
STATIC_ASSERT((u32)U32_MAX == U32_MAX);
STATIC_ASSERT((u32)-1 == U32_MAX);
MSVC_PRAGMA_WARNING_PUSH
    MSVC_PRAGMA_WARNING_DISABLE(4310)
    STATIC_ASSERT((u32)(1 + U32_MAX) == 0);
MSVC_PRAGMA_WARNING_POP

typedef unsigned long long u64;
#define U64_MAX 0xFFFFFFFFFFFFFFFFull
STATIC_ASSERT(sizeof(u64) == 8);
STATIC_ASSERT((u64)U64_MAX == U64_MAX);
STATIC_ASSERT((u64)-1 == U64_MAX);
MSVC_PRAGMA_WARNING_PUSH
    MSVC_PRAGMA_WARNING_DISABLE(4310)
    STATIC_ASSERT((u64)(1 + U64_MAX) == 0);
MSVC_PRAGMA_WARNING_POP



// windows gunk

#define K32FN(ret, fn) __declspec(dllimport) ret __stdcall fn
#define INVALID_HANDLE ((void*)-1)

void sys_exit(u32 exit_code) {
    K32FN(void, ExitProcess(u32));
    ExitProcess(exit_code);
}

void* sys_stderr(void) {
    K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-12);
    if (h == INVALID_HANDLE || h == 0){
        // if we do not even have stderr,
        //  we can not report errors,
        //  so just die with a hopefully
        //  noticable error code
        sys_exit((u32)-12);
    }
    return h;
}

void* sys_stdout(void) {
    K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-11);
    if (h == INVALID_HANDLE || h == 0){
        // todo: diagnostic on stderr
        sys_exit((u32)-11);
    }
    return h;
}

void* sys_stdin(void) {
    K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-10);
    if (h == INVALID_HANDLE || h == 0){
        // todo: diagnostic on stderr
        sys_exit((u32)-10);
    }
    return h;
}

// TODO
/*
__declspec(dllimport) 
int __stdcall 
ReadFile(
    void *hFile,
    void *lpBuffer,
    unsigned int nNumberOfBytesToRead,
    unsigned int *lpNumberOfBytesRead,
    void *lpOverlapped
);

__declspec(dllimport) 
int __stdcall 
WriteFile(
    void *hFile,
    const void *lpBuffer,
    unsigned int nNumberOfBytesToWrite,
    unsigned int *lpNumberOfBytesWritten,
    void *lpOverlapped
);
*/

// TODO
/*
u16 mem[U16_MAX + 1];
u16 get(u16 ptr) { 
    return mem[ptr];
}
u16 deref(u16 ptr_ptr) { 
    return mem[mem[ptr_ptr]]; 
}
void set(u16 ptr, u16 val) { 
    mem[ptr] = val;
}
void sub(u16 ptr, u16 val) { 
    mem[ptr] -= val;
}
void add(u16 ptr, u16 val) { 
    mem[ptr] += val;
}
u16 in(void) {
    int c = fgetc(M_STDIN);
    if (c != M_EOF)            
        return (u16)c;
    if (feof(M_STDIN))        
        return NIL;
    perror("fgetc");
    exit(1);
    return NIL;
}
void out(u16 val) {
    u8 ch = (u8)val;
    if (fputc(ch, M_STDOUT) != M_EOF) 
        return;
    perror("fputc");
    exit(1);
}
*/



// custom entry point

void entry(void) {
    (void) sys_stdin();
    (void) sys_stdout();
    (void) sys_stderr();
    sys_exit(0);
}



// TODO
/*
enum {
    ABORT   = 0xFFFE,
    EXIT    = 0xFFFF,
    NIL     = 0xFFFF,
};
u16 dispatch(u16 i) { 
    enum cases { start, read, print };
    enum defs { 
        buffer_ptr = 256, 
        buffer_end = 0xFFFF,
    };
    enum vars { begin, end, ch };
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
void entry(void) {
    u16 cmd = 0;
    while (cmd != EXIT && cmd != ABORT) {
        cmd = dispatch(cmd);
    }
}
*/