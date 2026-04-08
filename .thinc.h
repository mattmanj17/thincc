
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
