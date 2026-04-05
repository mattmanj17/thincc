
#include "thincc.h"
int main(void)
{
// --- constants ---

	u16 _0 = 0x0000;
	u16 _1 = 0x0001;

	u16 _CH_TAB = 0x0009;
	u16 _CH_LINE_FEED = 0x000A;
	u16 _CH_CARRIAGE_RETURN = 0x000D;

	u16 _CH_SPACE = 0x0020;
	u16 _CH_BANG = 0x0021;
	u16 _CH_DIGIT_0 = 0x0030;
	u16 _CH_TILDE = 0x007E;
	
	u16 _CH_a = 0x0061;
	u16 _CH_c = 0x0063;
	u16 _CH_e = 0x0065;
	u16 _CH_i = 0x0069;
	u16 _CH_l = 0x006C;
	u16 _CH_n = 0x006E;
	u16 _CH_o = 0x006F;
	u16 _CH_s = 0x0073;
	u16 _CH_t = 0x0074;

	u16 _U16_MOST = 0xFFFF;

	u16 _PTR_MOST = _U16_MOST;
	u16 _EOF = _U16_MOST;

	u16 _C_CH_FILE_SIZE_MAX = 0x8000;

	// we reserve a zero_page of 0x0100
	// u16's entries as a scratch buffer.
	// Any other named buffers need to be
	// after that.

// --- short lived vars ---

	// char

	u16 _ch = _0;

	// char *

	u16 _ch_ptr = _0;
	u16 _ch_ptr_rescan = _0;

	// counts

	u16 _c_ch_scrubbed = _0;
	u16 _c_ch_written = _0;
	u16 _c_ch_remaining = _0;

	u16 _c_line_breaks = _0;

// --- long lived vars ---

	// counts

	u16 _c_ch_file_size = _0;

// --- code ---

// _read_file
// _c_ch_file_size starts == 0
_read_file:
	_ch = in();
	if (_ch == _EOF)
		goto _scrub_ascii;

	if (_c_ch_file_size >= _C_CH_FILE_SIZE_MAX)
		goto _halt_failure;

	_ch_ptr = _PTR_MOST;
	_ch_ptr -= _c_ch_file_size;
	mem[_ch_ptr] = _ch;
	
	_c_ch_file_size += _1;
	goto _read_file;

// _scrub_ascii
// !!! expects _c_ch_file_size set
_scrub_ascii:
	_c_ch_scrubbed = _0;
	_ch_ptr = _PTR_MOST;
_scrub_ascii_loop:
	if (_c_ch_scrubbed == _c_ch_file_size)
		goto _write_file;

	_ch = mem[_ch_ptr];
	if (_ch < _CH_TAB)
		goto _bad_ch_ascii;
	if (_ch == _CH_TAB)
		goto _good_ch_ascii;
	if (_ch == _CH_LINE_FEED)
		goto _good_ch_ascii;
	if (_ch < _CH_CARRIAGE_RETURN)
		goto _bad_ch_ascii;
	if (_ch == _CH_CARRIAGE_RETURN)
		goto _good_ch_ascii;
	if (_ch < _CH_SPACE)
		goto _bad_ch_ascii;
	if (_ch <= _CH_TILDE)
		goto _good_ch_ascii;
	
_bad_ch_ascii:
	out(_CH_n);
	out(_CH_o);
	out(_CH_t);
	out(_CH_SPACE);
	out(_CH_a);
	out(_CH_s);
	out(_CH_c);
	out(_CH_i);
	out(_CH_i);
	out(_CH_BANG);
	out(_CH_LINE_FEED);
	goto _diag_at_ch_ptr;

_good_ch_ascii:
	_ch_ptr -= _1;
	_c_ch_scrubbed += _1;
	goto _scrub_ascii_loop;

// _bad_ch
// - count line breaks
// - print line number
// !!! expects _ch_ptr set
_diag_at_ch_ptr:
	_ch_ptr_rescan = _PTR_MOST;
	_c_line_breaks = 0;
_diag_loop_count_lines:
	_c_ch_remaining = _ch_ptr_rescan;
	_c_ch_remaining -= _ch_ptr;
	if (_c_ch_remaining == 0)
		goto _diag_print;
	
	// \n ?
	if (mem[_ch_ptr_rescan] != _CH_LINE_FEED)
		goto _diag_check_cr;
	_c_line_breaks += 1;
	goto _diag_continue_count_lines;

	// \r ?
_diag_check_cr:
	if (mem[_ch_ptr_rescan] != _CH_CARRIAGE_RETURN)
		goto _diag_continue_count_lines;
	_c_line_breaks += 1;
	
	// \r\n ?
	if (_c_ch_remaining < 2)
		goto _diag_continue_count_lines;
	_ch_ptr_rescan -= 1;
	if (mem[_ch_ptr_rescan] == _CH_LINE_FEED)
		goto _diag_continue_count_lines;
	_ch_ptr_rescan += 1;
	
	// continue
_diag_continue_count_lines:
	_ch_ptr_rescan -= 1;
	goto _diag_loop_count_lines;

	// print out line number
_diag_print:

	// 1 index the line number
	_c_line_breaks += _1;
	
	// void putu(u16 _c_line_breaks)
	// 
	// 	// max length of u16 in base 10 is 5
	// 	char zero_page[5];
	// 	char * _ch_ptr = &zero_page[4] + 1;
	// 	for (;;)
	// 	{
	// 		--_ch_ptr;
	// 		*_ch_ptr = (_c_line_breaks % 10) + '0';
	// 		_c_line_breaks /= 10;
	// 		if (!_c_line_breaks)
	// 			break;
	// 	}
	// 	while (_ch_ptr <= &zero_page[4])
	// 	{
	// 		out(*_ch_ptr);
	// 		++_ch_ptr;
	// 	}
	// 
	
	// char * _ch_ptr = &zero_page[4] + 1;
	_ch_ptr = 0x0005;
_diag_render:
	// --_ch_ptr;
	_ch_ptr -= _1;
	// *_ch_ptr = (_c_line_breaks % 10) + '0';
	mem[_ch_ptr] = _c_line_breaks;
	mem[_ch_ptr] %= 10;
	mem[_ch_ptr] += _CH_DIGIT_0;
	// _c_line_breaks /= 10;
	_c_line_breaks /= 10;
	// if (!_c_line_breaks) break;
	if (!(_c_line_breaks))
		goto _diag_print_at_line;

	goto _diag_render;

_diag_print_at_line:
	// printf("at line ");
	out(_CH_a);
	out(_CH_t);

	out(_CH_SPACE);

	out(_CH_l);
	out(_CH_i);
	out(_CH_n);
	out(_CH_e);

	out(_CH_SPACE);

_diag_print_lineno:
	// while (_ch_ptr <= &zero_page[4])
	if (_ch_ptr >= 5)
		goto _diag_print_lf;
	// out(*_ch_ptr);
	out(mem[_ch_ptr]);
	// ++_ch_ptr;
	_ch_ptr += 1;

	goto _diag_print_lineno;

_diag_print_lf:
	out(_CH_LINE_FEED);
	goto _halt_failure;


// _write_file
// !!! expects _c_ch_file_size set
_write_file:
	_c_ch_written = _0;
	_ch_ptr = _PTR_MOST;
_write_file_loop:
	if (_c_ch_written == _c_ch_file_size)
		goto _halt_success;

	out(mem[_ch_ptr]);
	_ch_ptr -= _1;
	_c_ch_written += _1;
	goto _write_file_loop;

//  --- spin loops ---

// for nicer debug experience

_halt_success:
	goto _halt_success;

_halt_failure:
	goto _halt_failure;
}
