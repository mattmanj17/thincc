
#include "_M_NBIT_U.h"

typedef unsigned char u8;
#define U8_MAX 0xFFu
M_VERIFY_NBIT_U(8);

typedef unsigned short u16;
#define U16_MAX 0xFFFFu
M_VERIFY_NBIT_U(16);

typedef unsigned int u32;
#define U32_MAX 0xFFFFFFFFu
M_VERIFY_NBIT_U(32);

typedef unsigned long long u64;
#define U64_MAX 0xFFFFFFFFFFFFFFFFu
M_VERIFY_NBIT_U(64);

void sys_exit(u32 exit_code) {
    __declspec(dllimport) void __stdcall ExitProcess(u32 exit_code);
    ExitProcess(exit_code);
}

void entry(void) {
    sys_exit(0);
}
