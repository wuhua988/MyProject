#ifndef _ttbase_h_
#define _ttbase_h_

#if defined(_WIN32) || defined(_WIN64)
	#define TT_IS_WINDOWS
#elif defined(__linux__)
	#define TT_IS_LINUX
#else
	#error This platform is not supported
#endif


#if defined(_M_IX86) || defined(__i386__) \
	|| defined(__x86_64) || defined(__x86_64__)
#define SURPPORT_X86_PLATFORM
#endif

#if defined(_M_IA64) || defined(_M_X64) || defined(_M_IX64)	\
	|| defined(__x86_64) || defined(__x86_64__)
#define SUPPROT_X64_PLATFORM
#endif


#if defined(_MSC_VER)
		#if defined(SUPPROT_X64_PLATFORM) || defined(_WIN64)
			#define TT_SYS_BITS		64
		#elif defined(WIN32)
			#define TT_SYS_BITS		32
		#elif defined(WIN16)
			#define TT_SYS_BITS		16
		#endif
#else
	#if defined(__LP64__)		\
	|| defined(_LP64)		\
	|| defined(__64BIT__)	\
	|| defined(__x86_64)	\
	|| defined(__x86_64__)	\
	|| defined(_LP64)		\
	|| defined(__LP64__)	\
	|| defined(__amd64)		\
	|| defined(__amd64__)	\
	|| (64 == __WORDSIZE)
		#define TT_SYS_BITS		64
	#else
		#define TT_SYS_BITS		32
	#endif
#endif




#endif //_ttbase_h_