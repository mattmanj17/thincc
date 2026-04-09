
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

void entry(void) {
    (void) sys_stdin();
    (void) sys_stdout();
    (void) sys_stderr();
    sys_exit(0);
}
