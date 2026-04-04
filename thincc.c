
#include "thincc.h"
int main(void)
{
// --- variables ---

	// constants

	u16 _0 = 0x0000;
	u16 _1 = 0x0001;

	u16 _ch_tab = 0x0009;
	u16 _ch_line_feed = 0x000A;
	u16 _ch_carriage_return = 0x000D;
	u16 _ch_space = 0x0020;
	u16 _ch_tilde = 0x007E;

	u16 _U16_MOST = 0xFFFF;
	u16 _U15_SUP = 0x8000;

	u16 _ADDR_MOST = _U16_MOST;
	u16 _EOF = _U16_MOST;
	u16 _FILE_SIZE_SUP = _U15_SUP;

	// scratch

	u16 _ch = _0;
	u16 _i = _0;
	u16 _j = _0;

	// globals

	u16 _file_size = _0;

// --- code ---

_read_file:
	_ch = in();
	if (_ch == _EOF)
		goto _scrub_ascii;

	if (_file_size >= _FILE_SIZE_SUP)
		goto _halt_failure;

	_i = _ADDR_MOST;
	_i -= _file_size;
	mem[_i] = _ch;
	
	_file_size += _1;
	goto _read_file;

_scrub_ascii:
	_i = _0;
	_j = _ADDR_MOST;
_scrub_ascii_loop:
	if (_i == _file_size)
		goto _write_file;

	_ch = mem[_j];
	if (_ch < _ch_tab)
		goto _bad_ch;
	if (_ch == _ch_tab)
		goto _good_ch_ascii;
	if (_ch == _ch_line_feed)
		goto _good_ch_ascii;
	if (_ch < _ch_carriage_return)
		goto _bad_ch;
	if (_ch == _ch_carriage_return)
		goto _good_ch_ascii;
	if (_ch < _ch_space)
		goto _bad_ch;
	if (_ch <= _ch_tilde)
		goto _good_ch_ascii;
	
	goto _bad_ch;

_good_ch_ascii:
	_j -= _1;
	_i += _1;
	goto _scrub_ascii_loop;

_bad_ch:
	goto _halt_failure;

_write_file:
	_i = _0;
	_j = _ADDR_MOST;
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
