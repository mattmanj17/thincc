#ifndef M_NBIT_U_H
#define M_NBIT_U_H

#include "_M_JOIN.h"
#include "_M_STATIC_ASSERT.h"

#define M_NBIT_U(n)      M_JOIN_2(u, n)
#define M_NBIT_U_MAX(n)  M_JOIN_3(U, n, _MAX)
#define M_VERIFY_NBIT_U(n)\
    M_STATIC_ASSERT(sizeof(M_NBIT_U(n)) == n/8);\
    M_STATIC_ASSERT((M_NBIT_U(n))M_NBIT_U_MAX(n) == M_NBIT_U_MAX(n));\
    M_STATIC_ASSERT((M_NBIT_U(n))-1 == M_NBIT_U_MAX(n));\
    __pragma(warning(push))\
        /* cast truncates constant value */\
        __pragma(warning(disable:4310))\
        M_STATIC_ASSERT((M_NBIT_U(n))(1 + M_NBIT_U_MAX(n)) == 0)\
    __pragma(warning(pop))

#endif // M_NBIT_U_H