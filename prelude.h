
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
typedef unsigned short u16;

// syscalls
u16 thinc_getc(void);
void thinc_putc(u16 arg_ch);
