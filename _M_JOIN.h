#ifndef M_JOIN_H
#define M_JOIN_H

#define M_JOIN_R(a,b)             a##b
#define M_JOIN_2(a,b)             M_JOIN_R(a,b)
#define M_JOIN_3(a,b,c)           M_JOIN_2(M_JOIN_2(a,b),c)
#define M_JOIN_4(a,b,c,d)         M_JOIN_2(M_JOIN_3(a,b,c),d)
#define M_JOIN_5(a,b,c,d,e)       M_JOIN_2(M_JOIN_4(a,b,c,d),e)
#define M_JOIN_6(a,b,c,d,e,f)     M_JOIN_2(M_JOIN_5(a,b,c,d,e),f)
#define M_JOIN_7(a,b,c,d,e,f,g)   M_JOIN_2(M_JOIN_6(a,b,c,d,e,f),g)
#define M_JOIN_8(a,b,c,d,e,f,g,h) M_JOIN_2(M_JOIN_7(a,b,c,d,e,f,g),h)

#endif // M_JOIN_H