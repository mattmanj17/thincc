
#include "prelude.h"
int main(void) {
    u16 i = 0;
    while (i != rc_exit && i != rc_abort) {
        i = run(i);
    }
    return (i == rc_abort);
}
