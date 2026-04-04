
#include "thincc.h"
int main(void)
{
	u16 _MEM_MOST = 0xFFFF;
	u16 _EOF = 0xFFFF;
	u16 _FILE_SIZE_MOST = 0x8000;

	u16 _x = 0;
	u16 _y = 0;
	u16 _file_size = 0x0000;
	
_read_file:
	_x = in();
	if (_x == _EOF)
		goto _success;

	if (_file_size >= _FILE_SIZE_MOST)
		goto _failure;

	_y = _MEM_MOST;
	_y -= _file_size;
	mem[_y] = _x;
	
	_file_size += 0x0001;
	goto _read_file;

_success:
	return 0;

_failure:
	return 1;
}
