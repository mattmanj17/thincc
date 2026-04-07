
// see: https://sourceforge.net/p/predef/wiki/Compilers/
#ifdef _MSC_VER
#else
#	error unsupported compiler
#endif

// see https://sourceforge.net/p/predef/wiki/Standards/
#ifndef __STDC__
#	error non-standard c compiler
#endif
#ifndef __STDC_VERSION__
#	error non-standard c compiler
#endif
#if __STDC_VERSION__ == 201112L // C11
#elif __STDC_VERSION__ == 201710L // C17/C18
#else
#	error unsupported c standard
#endif
#ifdef __cplusplus
#	error c++ compilers not supported
#endif

// see https://sourceforge.net/p/predef/wiki/OperatingSystems/
#ifdef _WIN64
#else
#	error unsupported os
#endif

// see https://sourceforge.net/p/predef/wiki/Architectures/
#ifdef _M_AMD64
#else
#	error unsupported architecture
#endif

// NOTE size confirmed in prelude.c
typedef unsigned short thinc_u16;

// some constants
enum {
	// EXIT_FAILURE
	thinc_exit_failure = 0xFFFE,

	// EXIT_SUCCESS
	thinc_exit_success = 0xFFFF,

	// EOF
	thinc_eof = 0xFFFF,
};

// opcodes
thinc_u16 thinc_load(thinc_u16 addr);
void thinc_store(thinc_u16 addr, thinc_u16 value);
void thinc_sub(thinc_u16 addr, thinc_u16 value);
void thinc_add(thinc_u16 addr, thinc_u16 value);

// syscalls
thinc_u16 thinc_getc(void);
void thinc_putc(thinc_u16 ch);

// entry point
thinc_u16 thinc_main(thinc_u16 instruction);
