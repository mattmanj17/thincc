
#include "thincc.h"
int main(void)
{
	u16 _in = 0x0000;
_loop:
	_in = in();
	// _in == eof?
	if (_in == 0xFFFF) 
		goto _halt;
	// > z ?
	if (_in > 0x007A) 
		goto _out;
	// < a ?
	if (_in < 0x0061) 
		goto _out;
	// to upper
	_in -= 0x0061;
	_in += 0x0041;
_out:
	out(_in);
	goto _loop;
_halt:
	goto _halt;
}
