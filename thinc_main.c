
#include "prelude.h"
#include "thinc.defs"
THINC_BEGIN
enum cmd {
	start,
	read,
	print,
	print_span,
};
enum {
	buffer_size = 0x8000,
	addr_buffer = thinc_addr_last - buffer_size + 1,
};

case start:
{
	thinc_store(0x0000, 0x0000);
	return read;
}
break;

case read:
{
	thinc_u16 v_c_ch_file_size_cur = thinc_load(0x0000);
	thinc_u16 v_ch = 0x0000;
	thinc_u16 v_ch_ptr = addr_buffer;

	// read
	v_ch = thinc_getc();
	if (v_ch == thinc_eof) {
		// NOTE v_file_size is sitting in g_arg_0
		return print;
	}

	// buffer exhasted
	if (v_c_ch_file_size_cur >= buffer_size) {
		return thinc_exit_failure;
	}

	// store
	v_ch_ptr += v_c_ch_file_size_cur;
	thinc_store(v_ch_ptr, v_ch);
	v_c_ch_file_size_cur += 0x0001;

	// recurse
	thinc_store(0x0000, v_c_ch_file_size_cur);
	return read;
}
break;

case print:
{

	// exit if not supposed to print any ch's
	if (thinc_load(0x0000) == 0x0000) {
		return thinc_exit_success;
	}

	// turn v_file_size into ch_ptr_last
	// NOTE we must dec by 1 to get the "last" ptr
	thinc_sub(0x0000, 0x0001);
	thinc_add(0x0000, addr_buffer);

	// move to ch_ptr_last
	thinc_store(0x0001, thinc_load(0x0000));

	// ch_ptr_first = file_buffer_start
	thinc_store(0x0000, addr_buffer);

	// call print_span
	return print_span;
}
break;

case print_span:
{
	thinc_u16 v_ch_ptr_first = thinc_load(0x0000);
	thinc_u16 v_ch_ptr_last = thinc_load(0x0001);

	if (v_ch_ptr_first > v_ch_ptr_last) {
		// done printing
		return thinc_exit_success;
	}

	// print
	thinc_putc(thinc_load(v_ch_ptr_first));

	// handle case where v_ch_ptr_last is the end of mem
	if (v_ch_ptr_first == thinc_addr_last) {
		return thinc_exit_success;
	}

	// inc
	v_ch_ptr_first += 0x0001;

	// recurse
	thinc_store(0x0000, v_ch_ptr_first);
	return print_span;
}
break;

default:
{
	return thinc_exit_failure;
}
break;
THINC_END
