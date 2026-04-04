
#include "thincc.h"
int main(void)
{
	u16 _H = 0x0048;
	u16 _W = 0x0057;
	u16 _lf = 0x000A;
	mem[0x0000] = _H;
	mem[0x0001] = _W;
	out(mem[0x0000]);
	out(mem[0x0001]);
	out(_lf);
}
