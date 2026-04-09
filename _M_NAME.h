#ifndef M_NAME_H
#define M_NAME_H

#include "_M_JOIN.h"

// M_NAME : name_0_1_2_3, name_4_5_6_7, etc
#define M_COUNTER __COUNTER__
#define M_NAME M_JOIN_8(name_,M_COUNTER,_,M_COUNTER,_,M_COUNTER,_,M_COUNTER)

#endif // M_NAME_H