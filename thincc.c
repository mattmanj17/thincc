
#include "_M_NBIT_U.h"

typedef unsigned char u8;
#define U8_MAX 0xFFu
M_VERIFY_NBIT_U(8);

typedef unsigned short u16;
#define U16_MAX 0xFFFFu
M_VERIFY_NBIT_U(16);

typedef unsigned int u32;
#define U32_MAX 0xFFFFFFFFul
M_VERIFY_NBIT_U(32);

typedef unsigned long long u64;
#define U64_MAX 0xFFFFFFFFFFFFFFFFull
M_VERIFY_NBIT_U(64);

#define M_K32FN(ret, fn) __declspec(dllimport) ret __stdcall fn
#define M_INVALID_HANDLE ((void*)-1)

void sys_exit(u32 exit_code) {
    M_K32FN(void, ExitProcess(u32));
    ExitProcess(exit_code);
}

void* sys_stderr(void) {
    M_K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-12);
    if (h == M_INVALID_HANDLE || h == 0){
        // if we do not even have stderr,
        //  we can not report errors,
        //  so just die with a hopefully
        //  noticable error code
        sys_exit((u32)-12);
    }
    return h;
}

void* sys_stdout(void) {
    M_K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-11);
    if (h == M_INVALID_HANDLE || h == 0){
        // todo: diagnostic on stderr
        sys_exit((u32)-11);
    }
    return h;
}

void* sys_stdin(void) {
    M_K32FN(void*, GetStdHandle(u32));
    void* h = GetStdHandle((u32)-10);
    if (h == M_INVALID_HANDLE || h == 0){
        // todo: diagnostic on stderr
        sys_exit((u32)-10);
    }
    return h;
}

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

void entry(void) {
    (void) sys_stdin();
    (void) sys_stdout();
    (void) sys_stderr();
    sys_exit(0);
}

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