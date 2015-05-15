#ifndef _tttypes_h_
#define _tttypes_h_

#include "ttbase.h"

#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif

#ifndef CONST
	#define CONST const
#endif

#ifndef VOID
	#define VOID void
#endif

#ifndef NULL
	#ifdef __cplusplus
		#define NULL 0
	#else
		#define NULL ((void*)0)
	#endif
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef __cplusplus
	#undef true
	#undef false
	#define true 1
	#define false 0
#endif

typedef int							BOOL;
typedef int							INT;
typedef unsigned int				UINT;
typedef short						SHORT;
typedef unsigned short				USHORT;
typedef long						LONG;
typedef unsigned long				ULONG;

typedef char						CHAR;
typedef wchar_t						WCHAR;

typedef signed char					INT8;
typedef unsigned char				UINT8;
typedef signed short				INT16;
typedef unsigned short				UINT16;

#if defined(_MSC_VER)
	#if 64==TT_SYS_BITS
		typedef signed __int32		INT32;
		typedef unsigned __int32	UINT32;
	#else
		typedef signed int			INT32;
		typedef unsigned int		UINT32;
	#endif

	typedef signed __int64			INT64;
	typedef unsigned __int64		UINT64;

	#define I64CONST(x)				x
	#define U64CONST(x)				x
#else
	#if 32==TT_SYS_BITS || 64==TT_SYS_BITS
		typedef int					INT32;
		typedef unsigned int		UINT32;
	#else
		typedef signed long			INT32;
		typedef unsigned long		UINT32;
	#endif
	typedef signed long long		INT64;
	typedef unsigned long long		UINT64;
	#define I64CONST(x)				x##LL
	#define U64CONST(x)				x##LLU
#endif

#if defined(__linux__)
	typedef unsigned char				BYTE;
	typedef unsigned short				WORD;
	typedef unsigned long				DWORD;
#endif

#if 16==TT_SYS_BITS
	typedef short					INT_PTR;
	typedef unsigned short			UINT_PTR;
#elif 32==TT_SYS_BITS
	#if defined(__linux__)
		typedef int					INT_PTR;
		typedef unsigned int		UINT_PTR;
	#endif
#elif 64==TT_SYS_BITS
	typedef INT64					INT_PTR;
	typedef UINT64					UINT_PTR;
#endif



#endif//_tttypes_h_