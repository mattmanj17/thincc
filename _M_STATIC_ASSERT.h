#ifndef M_STATIC_ASSERT_H
#define M_STATIC_ASSERT_H

#include "_M_NAME.h"

// "typedef char foo[-1]" is an error, so this magic
// acts as a portable static assert
#define M_STATIC_ASSERT(x) typedef char M_NAME[(x) ? 1 : -1]

#endif // M_STATIC_ASSERT_H