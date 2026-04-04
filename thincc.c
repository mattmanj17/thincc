
#include "thincc.h"
int main(void)
{
	// --- variables ---

	// constants

	u16 _0 = 0x0000;
	u16 _1 = 0x0001;

	u16 _U16_MOST = 0xFFFF;
	u16 _U15_SUP = 0x8000;

	u16 _NEG_1 = _U16_MOST;

	u16 _MEM_MOST = _NEG_1;
	u16 _EOF = _NEG_1;
	u16 _FILE_SIZE_MOST = _U15_SUP;

	// scratch

	u16 _x = _0;
	u16 _i = _0;
	u16 _j = _0;

	// globals

	u16 _file_size = _0;

	// --- code ---

_read_file:
	_x = in();
	if (_x == _EOF)
		goto _write_file;

	if (_file_size >= _FILE_SIZE_MOST)
		goto _halt_success;

	_i = _MEM_MOST;
	_i -= _file_size;
	mem[_i] = _x;
	
	_file_size += _1;
	goto _read_file;

_write_file:
	_i = _0;
	_j = _MEM_MOST;
_write_file_loop:
	if (_i == _file_size)
		goto _halt_success;
	
	out(mem[_j]);
	_j -= _1;
	_i += _1;
	goto _write_file_loop;

	//  --- spin loops ---

	// for nicer debug experience

_halt_success:
	goto _halt_success;

_halt_failure:
	goto _halt_failure;
}
