
// predef.h

// see https://sourceforge.net/p/predef/wiki/Home/


// enable/disable host compiler version check
#ifndef CHECK_HOST_COMPILER_VERSION
#	error CHECK_HOST_COMPILER_VERSION must be defined
#endif

#if CHECK_HOST_COMPILER_VERSION == 1
#elif CHECK_HOST_COMPILER_VERSION == 0
#else
#	error CHECK_HOST_COMPILER_VERSION must be 0 or 1
#endif


// compiler id/version
// see: https://sourceforge.net/p/predef/wiki/Compilers/
#ifdef _MSC_VER
#	if CHECK_HOST_COMPILER_VERSION
#		if _MSC_VER == 1944
#		else
#			error unsupported _MSC_VER
#		endif
#	endif
#else
#	error unsupported compiler
#endif


// __STDC__ / etc
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



// OS
// see https://sourceforge.net/p/predef/wiki/OperatingSystems/
#ifdef _WIN64
#else
#	error unsupported os
#endif



// ARCH
// see https://sourceforge.net/p/predef/wiki/Architectures/
#ifdef _M_AMD64
#else
#	error unsupported architecture
#endif
